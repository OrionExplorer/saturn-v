var computingAll = false;

var lastTime = 0;
var doMissionLog = false;
var missionLog = '';
var lastDP = 0;

var maxQAchieved = false;
var qZero = false;

var stableOrbitAchievedInfo = false;
var liftOffYawAchieved = false;

var towerClearInfo = false;
var liftOffInfo = false;

var autoPilotEnabled = true;

var currentFuelMass = 0;
var totalMass = 0;
var thrustNewtons = 0;
var currentAcceleration = 0;
var currentDistance = 0;
var currentVelocity = 0;
var currentVerticalVelocity = 0;
var currentHorizontalVelocity = 0;
var currentFuelBurn = 0;
var currentThrust = mainEngine.getThrust();

function calculateVelocityAfterTLI(sec) {
    //Credit to http://www.braeunig.us/apollo/apollo11-TLI.htm
    var e_rad = 0.381278;
    var mean_anomaly = (sec-(2*3600 + 47 * 60 + 34)) * 0.00000406681;
    var eccentric_anomaly = mean_anomaly + 0.977148 * Math.sin(e_rad);
    var true_anomaly = Math.acos((Math.cos(eccentric_anomaly) - 0.977148) / (1 - 0.977148 * Math.cos(eccentric_anomaly)));
    var radius = (288852900 * (1 - Math.pow(0.977148,2))) / (1 + 0.977148 * Math.cos(true_anomaly));
    var flight_path_angle = Math.atan((0.977148 * Math.sin(true_anomaly)) / (1 + 0.977148 * Math.cos(true_anomaly)));
    console.log('FPA: '+flight_path_angle);
    return Math.sqrt(3.986005e+14 * 288852900 * (1 - Math.pow(0.977148,2))) / (radius * Math.cos(flight_path_angle));
}

/**
 * execCommand
 * This is Voyager 7 main computer. Every single action that has to be done, 
 * must be executed through this method.
 * @param device - device
 * @param command - command
 * @param param1 - optional parameter
 * @param param2 - optional parameter
 * @param param3 - optional parameter
 * @return - positive or negative for executing command
 * 
 * ===START SEQUENCES===
 * 1. To start MAIN ENGINE (@mainEngine):
 *      - APS OFF
 *      - DPS OFF
 *      - S1 ATTACHED
 *      - S1 FUELED
 * 2. To start APS(@systemAPS):
 *      - MAIN ENGINE ON
 *      - DPS OFF
 *      - THRUST NULL
 * 3. To start DPS(@systemDPS):
 *      - MAIN ENGINE ON
 *      - APS OFF
 *      - THRUST NULL
 * 4. To change THRUST:
 *      - MAIN ENGINE ON
 *      - APS/DPS ON
 * 5. To attach S1(@systemS1):
 *      - MAIN ENGINE OFF
 * 6. To perform S1 fuel operation:
 *      - S1 ATTACHED
 * ===STOP SEQUENCES===
 * 1. To stop MAIN ENGINE:
 *      - APS OFF
 *      - DPS OFF
 *      - THRUST NULL
 *      - SPEED 0
 * 2. To stop APS:
 *      - THRUST NULL
 * 3. To stop DPS:
 *      - THRUST NULL
 * 4. To detach S1:
 *      - MAIN ENGINE OFF
 **/
function execCommand(device, command, param1, param2, param3) {
    var result;
    if(horizontalAltitudeEstablished != 1 && device != 'HORIZONTAL_ALTITUDE_PROGRAM') {
        result = false;
        updateInformation('ERROR: UNABLE TO PERFORM REQUESTED OPERATION DUE TO LOCAL HORIZONTAL ALTITUDE PROBLEM');
        return result;
    }

    switch(device) {
        case 'MAIN_ENGINE' : {
            switch(command) {
                case 'START' : {
                    if(mainEngine.engaged) {
                        result = false;
                        updateInformation('ERROR: MAIN ENGINE IS ENGAGED');
                    } else {
                        if((!systemAPS.engaged) && (!systemDPS.engaged) && (systemS1.attached) && (systemS1.getFuel() > 0)) {
                            mainEngine.doEngage();
                            result = true;
                            updateInformation('MAIN ENGINE ENGAGED');
                        } else {
                            result = false;
                            updateInformation('ERROR: UNABLE TO START MAIN ENGINE. CHECK CONFIGURATION');
                        }
                    }
                }
                break;
                case 'STOP' : {
                    if(!mainEngine.engaged) {
                        result = false;
                        updateInformation('ERROR: MAIN ENGINE IS DISENGAGED');
                    } else {
                        if((!systemAPS.engaged) && (!systemDPS.engaged) && (mainEngine.getThrust() == 0) && (currentSpeed == 0)) {
                            result = true;
                            mainEngine.doDisengage();
                            updateInformation('MAIN ENGINE DISENGAGED');

                        } else {
                            result = false;
                            updateInformation('ERROR: UNABLE TO STOP MAIN ENGINE. CHECK CONFIGURATION');
                        }
                    }
                }
                break;
            }
        }
        break;

        case 'S1' : {
            if(mainEngine.engaged && (systemAPS.engaged || systemDPS.engaged) && command != 'CENTER_ENGINE_CUTOFF') {
                result = false;
                updateInformation('ERROR: UNABLE TO PERFORM ANY S-IC OPERATION. CHECK CONFIGURATION');
            } else {
                switch(command) {
                    case 'TANK' : {
                        if(systemS1.getFuel() < systemS1.MAX_FUEL && systemS1.attached) {
                            result = true;
                            systemS1.setFuel(systemS1.MAX_FUEL);
                            updateInformation('PERFORMED S-IC FUEL OPERATION');
                        } else {
                            result = false;
                            updateInformation('ERROR: UNABLE TO PERFORM S-IC TANK OPERATION. CHECK CONFIGURATION');
                        }
                    }break;
                    case 'RELEASE' : {
                        if(systemS1.getFuel() > 0 && systemS1.attached) {
                            result = true;
                            systemS1.setFuel(0);
                            updateInformation('PERFORMED S-IC FUEL OPERATION');
                        } else {
                            result = false;
                            updateInformation('ERROR: UNABLE TO PERFORM S-IC RELEASE OPERATION. CHECK CONFIGURATION');
                        }
                    }break;
                    case 'ATTACH' : {
                        if(currentAltitude > 0) {
                            result = false;
                            updateInformation('ERROR: UNABLE TO PERFORM S-IC ATTACH OPERATION. CHECK CONFIGURATION');
                        } else {
                            result = true;
                            systemS1.doAttach();
                            updateInformation('S-IC STAGE ATTACHED');
                        }
                    }break;
                    case 'DETACH' : {
                        result = true;
                        systemS1.doDetach();
                        updateInformation('S-IC STAGE DETACHED');
                    }break;
                    case 'CENTER_ENGINE_CUTOFF' : {
                        if(systemS1.center_engine_available) {
                            result = true;
                            systemAPS.setThrust(systemAPS.getThrust()-20);
                            systemS1.center_engine_available = false;
                            updateInformation('S-IC CENTER ENGINE CUTOFF');
                        } else {
                            result = false;
                            updateInformation('ERROR: UNABLE TO PERFORM S-IC CENTER ENGINE CUTOFF. CHECK CONFIGURATION');
                        }
                    }break;
                }
            }
        }break;

        case 'S2' : {
            if(mainEngine.engaged && (systemAPS.engaged || systemDPS.engaged) && command != 'CENTER_ENGINE_CUTOFF') {
                result = false;
                updateInformation('ERROR: UNABLE TO PERFORM ANY S-II OPERATION. CHECK CONFIGURATION');
            } else {
                switch(command) {
                    case 'TANK' : {
                        if(systemS2.getFuel() < systemS2.MAX_FUEL && systemS2.attached) {
                            result = true;
                            systemS2.setFuel(systemS2.MAX_FUEL);
                            updateInformation('PERFORMED S-II FUEL OPERATION');
                        } else {
                            result = false;
                            updateInformation('ERROR: UNABLE TO PERFORM S-II TANK OPERATION. CHECK CONFIGURATION');
                        }
                    }break;
                    case 'RELEASE' : {
                        if(systemS2.getFuel() > 0 && systemS2.attached) {
                            result = true;
                            systemS2.setFuel(0);
                            updateInformation('PERFORMED S-II FUEL OPERATION');
                        } else {
                            result = false;
                            updateInformation('ERROR: UNABLE TO PERFORM S-II RELEASE OPERATION. CHECK CONFIGURATION');
                        }
                    }break;
                    case 'ATTACH' : {
                        if(currentAltitude > 0) {
                            result = false;
                            updateInformation('ERROR: UNABLE TO PERFORM S-II ATTACH OPERATION. CHECK CONFIGURATION');
                        } else {
                            result = true;
                            systemS2.doAttach();
                            updateInformation('S-II STAGE ATTACHED');
                        }
                    }break;
                    case 'DETACH' : {
                        result = true;
                        systemS2.doDetach();
                        updateInformation('S-II STAGE DETACHED');
                    }break;
                    case 'CENTER_ENGINE_CUTOFF' : {
                        if(systemS2.center_engine_available) {
                            result = true;
                            systemAPS.setThrust(systemAPS.getThrust()-20);
                            systemS2.center_engine_available = false;
                            updateInformation('S-II CENTER ENGINE CUTOFF');
                        }
                    }break;
                }
            }
        }break;

        case 'S3' : {
            if(mainEngine.engaged && (systemAPS.engaged || systemDPS.engaged)) {
                result = false;
                updateInformation('ERROR: UNABLE TO PERFORM ANY S-IVB OPERATION. CHECK CONFIGURATION');
            } else {
                switch(command) {
                    case 'TANK' : {
                        if(systemS3.getFuel() < systemS3.MAX_FUEL && systemS3.attached) {
                            result = true;
                            systemS3.setFuel(systemS3.MAX_FUEL);
                            updateInformation('PERFORMED S-IVB FUEL OPERATION');
                        } else {
                            result = false;
                            updateInformation('ERROR: UNABLE TO PERFORM S-IVB TANK OPERATION. CHECK CONFIGURATION');
                        }
                    }break;
                    case 'RELEASE' : {
                        if(systemS3.getFuel() > 0 && systemS3.attached) {
                            result = true;
                            systemS3.setFuel(0);
                            updateInformation('PERFORMED S-IVB FUEL OPERATION');
                        } else {
                            result = false;
                            updateInformation('ERROR: UNABLE TO PERFORM S-IVB RELEASE OPERATION. CHECK CONFIGURATION');
                        }
                    }break;
                    case 'ATTACH' : {
                        if(currentAltitude > 0) {
                            result = false;
                            updateInformation('ERROR: UNABLE TO PERFORM S-IVB ATTACH OPERATION. CHECK CONFIGURATION');
                        } else {
                            result = true;
                            systemS3.doAttach();
                            updateInformation('S-IVB STAGE ATTACHED');
                        }
                    }break;
                    case 'DETACH' : {
                        result = true;
                        systemS3.doDetach();
                        updateInformation('S-IVB STAGE DETACHED');
                    }break;
                    case 'RESTART' : {
                        if(currentSystemStage.id == systemS3.id) {
                            result = true;
                            systemS3.burn_start = false;
                            systemS3.burn_time = 0;
                            updateInformation('S-IVB BURN RESTART');
                        } else {
                            result = false;
                            updateInformation('ERROR: UNABLE TO RESTART S-IVB BURN. CHECK CONFIGURATION');
                        }
                    }break;
                }
            }
        }break;

        case 'APS' : {
            switch(command) {
                case 'START' : {
                    if(systemAPS.engaged) {
                        result = false;
                        updateInformation('ERROR: APS SYSTEM IS ENGAGED');
                    } else {
                        if((mainEngine.engaged) && (!systemDPS.engaged) && (mainEngine.getThrust() == 0)) {
                            systemAPS.doEngage();
                            result = true;
                            updateInformation('APS SYSTEM ENGAGED');
                            if(!computingAll) {
                                computeAll();
                                computingAll = true;
                                updateInformation('APS SYSTEM ENGAGED. IGNITION SEQUENCE START');
                                execCommand('THRUST', 'INCREASE', 15, true);
                            }
                        } else {
                            result = false;
                            updateInformation('ERROR: UNABLE TO START APS SYSTEM. CHECK CONFIGURATION');
                        }
                    }
                }
                break;
                case 'STOP' : {
                    if(!systemAPS.engaged) {
                        result = false;
                        updateInformation('ERROR: APS SYSTEM IS DISENGAGED');
                    } else {
                        if(systemAPS.getThrust() == 0) {
                            systemAPS.doDisengage();
                            result = true;
                            updateInformation('APS SYSTEM DISENGAGED');
                        } else {
                            result = false;
                            updateInformation('ERROR: UNABLE TO STOP APS SYSTEM. CHECK CONFIGURATION');
                        }
                    }
                }
                break;
            }
        }
        break;

        case 'THRUST' : {
            if(((!mainEngine.engaged)&&( (!systemDPS.engaged) || (!systemAPS.engaged) ))) {
                result = false;
                updateInformation('ERROR: UNABLE TO CHANGE THRUST. CHECK CONFIGURATION');
            } else {
                switch(command) {
                    case 'FULL' : {
                        if(systemDPS.engaged) {
                            systemDPS.setThrust(MAX_THRUST);
                            result = true;
                            if(param2 == false) {
                                updateInformation('DPS THRUST SET TO 100%');
                            }
                        } else if(systemAPS.engaged) {
                            result = true;
                            systemAPS.setThrust(MAX_THRUST);
                            if(param2 == false) {
                                updateInformation('APS THRUST SET TO 100%');
                            }
                        } else {
                            result = false;
                            updateInformation('ERROR: MPS SYSTEM IS IDLE');
                        }
                    }
                    break;
                    case 'NULL' : {
                        if(systemDPS.engaged) {
                            systemDPS.setThrust(0);
                            result = true;
                            if(param2 == false) {
                                updateInformation('DPS THRUST TERMINATED');
                            }
                        } else if(systemAPS.engaged) {
                            result = true;
                            systemAPS.setThrust(0);
                            if(param2 == false) {
                                updateInformation('APS THRUST TERMINATED');
                            }
                        } else {
                            result = false;
                            updateInformation('ERROR: MPS SYSTEM IS IDLE');
                        }
                    }
                    break;
                    case 'INCREASE' : {
                        if(systemDPS.engaged) {
                            if(systemDPS.getThrust() + param1 <= MAX_THRUST) {
                                systemDPS.setThrust(systemDPS.getThrust() + param1);
                                result = true;
                                if(param2 == false) {
                                    updateInformation('DPS THRUST SET TO '+systemDPS.getThrust()+'%');
                                }
                            } else {
                                result = false;
                                if(param2 == false) {
                                    updateInformation('ERROR: UNABLE TO INCREASE THRUST TO '+parseInt(systemDPS.getThrust() + param1)+'%');
                                }
                            }
                        } else if(systemAPS.engaged) {
                            if(systemAPS.getThrust() + param1 <= MAX_THRUST) {
                                systemAPS.setThrust(systemAPS.getThrust() + param1);
                                result = true;
                                if(param2 == false) {
                                    updateInformation('APS THRUST SET TO '+systemAPS.getThrust()+'%');
                                }
                            } else {
                                result = false;
                                if(param2 == false) {
                                    updateInformation('ERROR: UNABLE TO INCREASE THRUST TO '+parseInt(systemAPS.getThrust() + param1)+'%');
                                }
                            }
                        } else {
                            result = false;
                            updateInformation('ERROR: MPS SYSTEM IS IDLE');
                        }
                    }
                    break;
                    case 'DECREASE' : {
                        if(systemDPS.engaged) {
                            if(systemDPS.getThrust() - param1 >= 0) {
                                systemDPS.setThrust(systemDPS.getThrust() - param1);
                                result = true;
                                if(param2 == false) {
                                    updateInformation('DPS THRUST SET TO '+systemDPS.getThrust()+'%');
                                }
                            } else {
                                result = false;
                                if(param2 == false) {
                                    updateInformation('ERROR: UNABLE TO DECREASE THRUST TO '+parseInt(systemDPS.getThrust() - param1)+'%');
                                }
                            }
                        } else if(systemAPS.engaged) {
                            if(systemAPS.getThrust() - param1 >= 0) {
                                systemAPS.setThrust(systemAPS.getThrust() - param1);
                                result = true;
                                if(param2 == false) {
                                    updateInformation('APS THRUST SET TO '+systemAPS.getThrust()+'%');
                                }
                            } else {
                                result = false;
                                if(param2 == false) {
                                    updateInformation('ERROR: UNABLE TO DECREASE THRUST TO '+parseInt(systemAPS.getThrust() - param1)+'%');
                                }
                            }
                        } else {
                            result = false;
                            updateInformation('ERROR: MPS SYSTEM IS IDLE');
                        }
                    }
                    break;
                }
            }
        }
        break;

        case 'HG_ANTENNA' : {
            if(mainEngine.getThrust() > 0) {
                result = false;
                updateInformation('ERROR: UNABLE TO PERFORM ANTENNA MANEUVER. CHECK CONFIGURATION');
            } else {
                switch(command) {
                    case 'ACTIVATE' : {
                        highGainAntenna.doEngage();
                        result = true;
                        updateInformation('HIGH-GAIN ANTENNA ACTIVATED');
                    }
                    break;
                    case 'DEACTIVATE' : {
                        highGainAntenna.doDisengage();
                        result = true;
                        updateInformation('HIGH-GAIN ANTENNA DEACTIVATED');
                    }
                    break;
                }
            }
        }
        break;

        case 'LG_ANTENNA' : {
            if(mainEngine.getThrust() > 0) {
                result = false;
                updateInformation('ERROR: UNABLE TO PERFORM ANTENNA MANEUVER. CHECK CONFIGURATION');
            } else {
                switch(command) {
                    case 'ACTIVATE' : {
                        lowGainAntenna.doEngage();
                        result = true;
                        updateInformation('LOW-GAIN ANTENNA ACTIVATED');
                        }
                        break;
                    case 'DEACTIVATE' : {
                        lowGainAntenna.doDisengage();
                        result = true;
                        updateInformation('LOW-GAIN ANTENNA DEACTIVATED');
                    }
                    break;
                }
            }
        }
        break;

        case 'PITCH_ROLL_PROGRAM' : {
            if(!mainEngine.engaged || !systemAPS.engaged || (currentAltitude <= 0)) {
                result = false;
                updateInformation('ERROR: UNABLE TO START PITCH/ROLL PROGRAM. CHECK CONFIGURATION');
            } else {
                switch(command) {
                    case 'START' : {
                        result = true;
                        if(param1 == 'PITCH') {
                            pitchProgram.start();
                            updateInformation('PITCH PROGRAM STARTED');
                        } else if(param1 == 'ROLL') {
                            rollProgram.start();
                            updateInformation('ROLL PROGRAM STARTED');
                        }
                    }break;
                    case 'STOP' : {
                        result = true;
                        if(param1 == 'PITCH') {
                            pitchProgram.stop();
                            updateInformation('PITCH PROGRAM STOPPED');
                        } else if(param1 == 'ROLL') {
                            rollProgram.stop();
                            updateInformation('ROLL PROGRAM STOPPED');
                        }
                    }break;
                }
            }
        }
        break;

        case 'YAW_PROGRAM' : {
            if(!mainEngine.engaged || !systemAPS.engaged || (currentAltitude <= 0)) {
                result = false;
                updateInformation('ERROR: UNABLE TO START YAW PROGRAM. CHECK CONFIGURATION');
            } else {
                switch(command) {
                    case 'START' : {
                        result = true;
                        if(param1 == 'YAW') {
                            yawProgram.start();
                            updateInformation('YAW PROGRAM STARTED');
                        }
                    }break;
                    case 'STOP' : {
                        result = true;
                        if(param1 == 'YAW') {
                            yawProgram.stop();
                            updateInformation('YAW PROGRAM STOPPED');
                        }
                    }break;
                }
            }
        }
        break;

        case 'LET' : {
            if(!mainEngine.engaged || !systemAPS.engaged || (currentAltitude <= 0) || (!launchEscapeTowerReady)) {
                result = false;
                updateInformation('ERROR: UNABLE TO ENGAGE LAUNCH ESCAPE TOWER SYSTEM. CHECK CONFIGURATION');
            } else {
                switch(command) {
                    case 'JETTISON' : {
                        result = true;
                        systemS3.instrumentMass -= 4200;
                        launchEscapeTowerReady = false;
                        updateInformation('LAUNCH ESCAPE TOWER JETTISONED');
                    }break;
                }
            }
        }
        break;

        case 'HORIZONTAL_ALTITUDE_PROGRAM' : {
            if(!mainEngine.engaged || (currentAltitude <= 0)) {
                result = false;
                updateInformation('ERROR: UNABLE TO START LOCAL HORIZONTAL ALTITUDE MANEUVER. CHECK CONFIGURATION');
            } else {
                switch(command) {
                    case 'START' : {
                        if(horizontalAltitudeProgram.currentStep < horizontalAltitudeProgram.steps.length-1) {
                            result = true;
                            horizontalAltitudeProgram.currentStep++;
                            horizontalAltitudeProgram.start();
                            updateInformation('LOCAL HORIZONTAL ALTITUDE MANEUVER STARTED (MODE '+horizontalAltitudeProgram.currentStep+')');
                        } else {
                            result = false;
                            updateInformation('ERROR: NO FURTHER PROGRAM STEPS FOUND');
                        }
                        
                    }break;
                    case 'STOP' : {
                        result = true;
                        horizontalAltitudeProgram.stop();
                        updateInformation('LOCAL HORIZONTAL ALTITUDE MANEUVER STOPPED (MODE '+horizontalAltitudeProgram.currentStep+')');
                    }break;
                }
            }
        }
        break;
    }

    if(systemAPS.engaged) {
        mainEngine.setThrust(systemAPS.getThrust());
    } else if(systemDPS.engaged) {
        mainEngine.setThrust(systemDPS.getThrust());
    }

    console.log('execCommand('+device+', '+command+', '+param1+', '+param2+')::'+result);
    return result;
}

function getDynamicPressureForce(altitude) {
    var currentTemperature = 288.15 - (0.0065 * altitude);
    var gasConstant = 8.3144621;
    var currentAtmosphericPressure = 1013.25 * Math.exp((-1*(0.0289644*currentAO.getGravityValue(altitude)*altitude))/(gasConstant*currentTemperature));
    var currentAirDestiny = currentAtmosphericPressure/(gasConstant*currentTemperature);
    var dynamicPressure = ((1/2)*currentAirDestiny)*Math.pow(lastVelocity, 2);
    var result = dynamicPressure/10;

    if(maxQAchieved) {
        if(qZero) {
            return 0;
        } else {
            if(Math.round(result) == 0) {
                qZero = true;
                return 0;
            } else {
                return Math.round(result*10)/10;
            }
        }
    } else {
        if(result >= lastDP) {
            lastDP = result;
            return Math.round(result*10)/10;
        } else {
            maxQAchieved = true;
            updateInformation('MAXIMUM DYNAMIC PRESSURE');
            lastDP = Math.round(lastDP*10)/10;
            return lastDP;
        }
    }
}
var currentPitchMod = 0;
function getPitchStep() {
    var seconds = pitchProgram.running_time;
    var result = 0;

    if(seconds >= 0 && seconds < 29) {
        result = 0.8965517;
    }
    //do 00:01:00 mamy +26.896551 = 26.896551
    if(seconds >= 29 && seconds < 69) {
        result = 0.5212500;
    }
    //do 00:01:39 mamy +20.85 = 47.746551
    if(seconds >= 69 && seconds < 129) {
        result = 0.3908333;
    }
    //do 00:02:39 mamy +23.449998 = 71.196549
    if(seconds >= 129 && seconds < 209) {
        result = -0.1762500;
    }
    //do 00:03:59 mamy -14.1 = 57.096549
    if(seconds >= 209 && seconds < 359) {
        result = 0.0975067;
    }
    //do 00:06:29 mamy +7.973576 = 65.070125
    if(seconds >= 359 && seconds < 518) {
        result = 0.0996697;
    }
    //do 00:08:59 mamy +16.618005 = 83.995895
    if(seconds >= 509 && seconds < 559) {
        result = 0.1190400;
    }
    //mamy +2.659441 = 91
    currentPitchMod = result;
    return result;
}

function getRollStep() {
    return 0.94;
}

function getYawStep() {
    if(currentYaw >= destYaw) {
        liftOffYawAchieved = true;
    }

    if(currentAltitude <= 130 && !liftOffYawAchieved) {
        return 0.15625;
    } else if(currentAltitude > 130) {
        return -0.15625;
    }

    return 0;
}

function updateView() {
    updateHUD('yawButton', yawProgram.getPossibleAction());
    updateHUD('rollButton', rollProgram.getPossibleAction());
    updateHUD('pitchButton', pitchProgram.getPossibleAction());
    updateHUD('voyager7_pitch', Math.round(currentPitch)+'° / '+destPitch+'°');
    updateHUD('voyager7_rollAzimuth', Math.round(90-currentRoll)+'° / '+destRoll+'°');
    updateHUD('voyager7_yaw', Math.round(currentYaw*10)/10+'°');

    updateHUD('voyager7_s1_fuel', Math.round(systemS1.getFuel())+' KG');
    updateHUD('voyager7_s1_burnTime', systemS1.getBurnTime()+' S');
    updateHUD('voyager7_s1_status', systemS1.getAttached());
    updateHUD('voyager7_s2_fuel', Math.round(systemS2.getFuel())+' KG');
    updateHUD('voyager7_s2_burnTime', systemS2.getBurnTime()+' S');
    updateHUD('voyager7_s2_status', systemS2.getAttached());
    updateHUD('voyager7_s3_fuel', Math.round(systemS3.getFuel())+' KG');
    updateHUD('voyager7_s3_burnTime', systemS3.getBurnTime()+' S');
    updateHUD('voyager7_s3_status', systemS3.getAttached());

    if(systemS1.getFuel() > 0) {
        updateHUD('systemS1FuelButton', 'RELEASE');
    }
    if(systemS2.getFuel() > 0) {
        updateHUD('systemS2FuelButton', 'RELEASE');
    }
    if(systemS3.getFuel() > 0) {
        updateHUD('systemS3FuelButton', 'RELEASE');
    }

    if(missionTime > 0) {
        enableThrustChange();
    }

    //MAIN ENGINE DISENGAGED
    if(!mainEngine.engaged) {
        updateHUD('mainEngineEngageButton', 'ENGAGE');//, 2000, 'enableEl("'+id+'")');
    } else {
        updateHUD('mainEngineEngageButton', 'DISENGAGE');//, 2000, 'enableEl("'+id+'")');
    }
    //APS DISENGAGED
    if(!systemAPS.engaged) {
        updateHUD('systemAPSButton', 'ENGAGE');
    } else {
        updateHUD('systemAPSButton', 'DISENGAGE');
    }
    //00:02:15
    if(!systemS1.center_engine_available) {
        disableEl('systemS1CenterEngineCutoffButton');
    }
    //00:02:41
    if(!systemS1.attached) {
        disableEl('systemS1ActionButton');
        //updateHUD('systemS1ActionButton', 'STAGED');// 3000, 'disableEl("systemS1ActionButton")');
    }
    
    //00:03:17
    if(!launchEscapeTowerReady) {
        disableEl('towerJettisonButton');
        disableEl('towerJettisonEngageButton');
    }
    
    //00:07:41
    if(!systemS2.center_engine_available) {
        disableEl('systemS2CenterEngineCutoffButton');
    }
    
    //00:09:08
    if(!systemS2.attached) {
        disableEl('systemS2ActionButton');
        //updateHUD('systemS2ActionButton', 'STAGED');//, 3000, 'disableEl("systemS2ActionButton")');
    }
}
function autoPilot(real_second) {
    var second = Math.round(real_second);
//    if(currentYaw >= destYaw && yawProgram.running) {
//        execCommand('YAW_PROGRAM', 'STOP', 'YAW');
//        updateHUD('yawButton', yawProgram.getPossibleAction());
//    }
//    if(stableOrbitAchievedInfo) {
//        if(mainEngine.getThrust() == 100) {
//            execCommand('THRUST', 'NULL', null, false);
//            updateHUD('systemAPSButton', 'DISENGAGING...');
//            disableEl('systemAPSButton');
//            execCommand('APS', 'STOP');
//            updateHUD('systemAPSButton', 'ENGAGE', 2000, 'enableEl("systemAPSButton")');
//        }
//    }

    if(currentYaw == 0 && liftOffYawAchieved && yawProgram.running) {
        execCommand('YAW_PROGRAM', 'STOP', 'YAW');
        updateHUD('yawButton', yawProgram.getPossibleAction());
    }

    if((90-currentRoll) <= destRoll && rollProgram.running) {
        execCommand('PITCH_ROLL_PROGRAM', 'STOP', 'ROLL');
        updateHUD('rollButton', rollProgram.getPossibleAction());
    }

    if(currentPitch >= destPitch && pitchProgram.running) {
        execCommand('PITCH_ROLL_PROGRAM', 'STOP', 'PITCH');
        updateHUD('pitchButton', pitchProgram.getPossibleAction());
    }

    if(horizontalAltitudeEstablished == 0 && !horizontalAltitudeProgram.running) {
        execCommand('HORIZONTAL_ALTITUDE_PROGRAM', 'START');
        updateHUD('horizontalAltitudeButton', horizontalAltitudeProgram.getPossibleAction());//, horizontalAltitudeProgram.steps[horizontalAltitudeProgram.currentStep].duration*1000, 'enableEl("'+id+'")');
    } else if(horizontalAltitudeEstablished == 1 && horizontalAltitudeProgram.running) {
        execCommand('HORIZONTAL_ALTITUDE_PROGRAM', 'STOP');
        updateHUD('horizontalAltitudeButton', horizontalAltitudeProgram.getPossibleAction());//, horizontalAltitudeProgram.steps[horizontalAltitudeProgram.currentStep].duration*1000, 'enableEl("'+id+'")');
    }

    switch(second) {
        case 1: {
            if(!yawProgram.running) {
                execCommand('YAW_PROGRAM', 'START', 'YAW');
                updateHUD('yawButton', yawProgram.getPossibleAction());
            }
        }
        break;

        case 13: {
            if(!rollProgram.running) {
                execCommand('PITCH_ROLL_PROGRAM', 'START', 'ROLL');
                updateHUD('rollButton', rollProgram.getPossibleAction());
            }
        }
        break;

        case 31: {
            if(!pitchProgram.running) {
                execCommand('PITCH_ROLL_PROGRAM', 'START', 'PITCH');
                updateHUD('pitchButton', pitchProgram.getPossibleAction());
            }
        }
        break;

        case 135: {
            if(systemS1.center_engine_available) {
                //execCommand('THRUST', 'DECREASE', 20, true);
                execCommand('S1', 'CENTER_ENGINE_CUTOFF');
                disableEl('systemS1CenterEngineCutoffButton');
                updateInformation('S-IC CENTER ENGINE CUTOFF');
            }
        }
        break;

        case 162: {
            if(mainEngine.getThrust() == 80 && currentSystemStage.id == 1) {
                execCommand('THRUST', 'NULL', null, false);
                updateHUD('voyager7_s1_status', systemS1.getAttached());
                updateHUD('systemAPSButton', 'DISENGAGING...');
                disableEl('systemAPSButton');
                execCommand('APS', 'STOP');
                updateHUD('systemAPSButton', 'ENGAGE', 2000, 'enableEl("systemAPSButton")');
                execCommand('S1', 'DETACH');
                updateHUD('systemS1ActionButton', 'STAGING...');
                disableEl('systemS1ActionButton');
                updateHUD('systemS1ActionButton', 'OUTBOARD', 3000, 'disableEl("systemS1ActionButton")');
                updateHUD('voyager7_s1_thrust', '0 N');
            }
        }
        break;

        case 165: {
            if(mainEngine.getThrust() == 0) {
                updateHUD('systemAPSButton', 'ENGAGING...');
                disableEl('systemAPSButton');
                execCommand('APS', 'START');
                updateHUD('systemAPSButton', 'DISENGAGE', 2000, 'enableEl("systemAPSButton")');
                execCommand('THRUST', 'FULL', null, false);
                updateView();
            }
        }
        break;

        case 197: {
            if(launchEscapeTowerReady) {
                execCommand('LET', 'JETTISON');
                disableEl('towerJettisonButton');
                disableEl('towerJettisonEngageButton');
                
            }
        }
        break;

        case 460: {
            if(systemS2.center_engine_available) {
                //execCommand('THRUST', 'DECREASE', 20, true);
                execCommand('S2', 'CENTER_ENGINE_CUTOFF');
                disableEl('systemS2CenterEngineCutoffButton');
                updateInformation('S-II CENTER ENGINE CUTOFF');
            }
        }
        break;

        case 500: {
            if(mainEngine.getThrust() > 60) {
                execCommand('THRUST', 'DECREASE', 20, true);
            }
        }
        break;
        
        case 548: {
            if(mainEngine.getThrust() == 60 && currentSystemStage.id == 2) {
                execCommand('THRUST', 'NULL', null, false);
                updateHUD('voyager7_s2_status', systemS2.getAttached());
                updateHUD('systemAPSButton', 'DISENGAGING...');
                disableEl('systemAPSButton');
                execCommand('APS', 'STOP');
                updateHUD('systemAPSButton', 'ENGAGE', 2000, 'enableEl("systemAPSButton")');
                execCommand('S2', 'DETACH');
                updateHUD('systemS2ActionButton', 'STAGING...');
                disableEl('systemS2ActionButton');
                updateHUD('systemS2ActionButton', 'OUTBOARD', 3000, 'disableEl("systemS2ActionButton")');
                updateHUD('voyager7_s2_thrust', '0 N');
                updateView();
            }
        }
        break;

        case 552: {
            if(mainEngine.getThrust() == 0) {
                updateHUD('systemAPSButton', 'ENGAGING...');
                disableEl('systemAPSButton');
                execCommand('APS', 'START');
                updateHUD('systemAPSButton', 'DISENGAGE', 2000, 'enableEl("systemAPSButton")');
                execCommand('THRUST', 'FULL', null, false);
            }
        }
        break;

        case 701: {
            if(mainEngine.getThrust() == 100) {
                execCommand('THRUST', 'NULL', null, false);
                updateHUD('systemAPSButton', 'DISENGAGING...');
                disableEl('systemAPSButton');
                execCommand('APS', 'STOP');
                updateHUD('systemAPSButton', 'ENGAGE', 2000, 'enableEl("systemAPSButton")');
            }
        }
        break;
    }
}

function timeBasedChange(seconds) {
    switch(seconds) {
        case 715 : {
            execCommand('THRUST', 'NULL', null, false);
            updateHUD('systemAPSButton', 'DISENGAGING...');
            disableEl('systemAPSButton');
            execCommand('APS', 'STOP');
            updateHUD('systemAPSButton', 'ENGAGE', 2000, 'enableEl("systemAPSButton")');
            horizontalAltitudeEstablished = 0;
        }break;
        case 10219 : {
            execCommand('THRUST', 'NULL', null, false);
            updateHUD('systemAPSButton', 'DISENGAGING...');
            disableEl('systemAPSButton');
            execCommand('APS', 'STOP');
            updateHUD('systemAPSButton', 'ENGAGE', 2000, 'enableEl("systemAPSButton")');
            horizontalAltitudeEstablished = 0;
        }
    }
}

function computeAll() {
    currentFuelMass = 0;
    totalMass = 0;
    thrustNewtons = 0;
    currentAcceleration = 0;
    currentDistance = 0;
    currentVelocity = 0;
    currentVerticalVelocity = 0;
    currentHorizontalVelocity = 0;
    currentFuelBurn = 0;
    currentThrust = mainEngine.getThrust();
    var timeMod = (1000/timeInterval);
    var timeTick = (timeInterval/1000);
    var dynamicPressure = 0;

    if(currentAltitude >= 130 && !towerClearInfo) {
        updateInformation('TOWER CLEARED');
        towerClearInfo = true;
    }

    if(!systemS1.attached) {
        if(systemS2.attached) {
            updateHUD('voyager7_s1_thrust', '0 N');
            currentSystemStage = systemS2;
        }
    }

    if(!systemS2.attached) {
        if(systemS3.attached) {
            updateHUD('voyager7_s2_thrust', '0 N');
            currentSystemStage = systemS3;
        }
    }

    if(!systemS3.attached && !systemS2.attached && !systemS1.attached) {
        currentSystemStage = systemNULL;
    }

    if(currentSystemStage.burn_start == false) {
        if(currentThrust == 100) {
            var newDate = new Date();
            var currentEpoch = newDate.getTime();
            delete newDate;
            currentSystemStage.burn_start = currentEpoch;
        }
    } else {
        if(systemAPS.engaged) {
            currentSystemStage.burn_time += timeTick;
        }
    }

    currentFuelMass = currentSystemStage.getFuel();

    if(systemS1.attached) {
        totalMass += systemS1.getTotalMass();
    }

    if(systemS2.attached) {
        totalMass += systemS2.getTotalMass();
    }

    if(systemS3.attached) {
        totalMass += systemS3.getTotalMass();
    }

    missionTime += timeTick;

    if(autoPilotEnabled) {
        autoPilot(missionTime);
    }

    if(missionTime >= -0.4) {
        if(!liftOffInfo) {
            execCommand('THRUST', 'INCREASE', 85, true);
            enableThrustChange();
            updateInformation('LIFT OFF');
            liftOffInfo = true;
        }
    }

    if(currentAltitude == 0) {
        ascendingTime = 0;
    }

    if(currentFuelMass > 0) {
        currentFuelBurn = ((currentThrust * currentSystemStage.MAX_FUEL_BURN) / 100)/timeMod;
        currentSystemStage.setFuel(currentFuelMass - currentFuelBurn);
    } else {
        currentFuelMass = 0;
    }

    if(currentFuelBurn > 0) {
        if(currentSystemStage.VARIABLE_THRUST && (currentSystemStage.INITIAL_THRUST < currentSystemStage.MAX_THRUST_N)) {

            if(currentThrust == 100) {
                currentSystemStage.INITIAL_THRUST += (currentSystemStage.THRUST_STEP)/(timeMod);
                thrustNewtons = (currentThrust * currentSystemStage.INITIAL_THRUST) / 100;
            } else {
                thrustNewtons = (currentThrust * currentSystemStage.INITIAL_THRUST) / 100;
            }
 
       } else {
            thrustNewtons = (currentThrust * currentSystemStage.MAX_THRUST_N) / 100;
        }
    } else {
        thrustNewtons = 0;
    }

    var Fw = 0;
    if(mainEngine.getThrust() >= 60) {
         dynamicPressure = getDynamicPressureForce(currentAltitude);
    }

    var dynamicPressureNewtons = (dynamicPressure)*4.44;

    if(!maxQAchieved) {
        Fw = thrustNewtons - (totalMass*currentAO.getGravityValue(currentAltitude));
    } else {
        Fw = thrustNewtons - (totalMass*(currentAO.getGravityValue(currentAltitude)/(timeMod))) + dynamicPressureNewtons*1000;
    }
    currentAcceleration = (Fw/totalMass);

    if(currentAcceleration < 0 && currentAltitude <= 0) {
        currentAcceleration = 0;
    }

    if(lastVelocity >= currentAO.getOrbitalSpeed(currentAltitude)) {
        if(currentAcceleration < 0) {
            currentAcceleration = 0;
        }
    }

    currentVelocity = lastVelocity+(currentAcceleration/(timeMod));
    if(currentPitch > 0) {
        var rad2deg = currentPitch * Math.PI/180;

        currentVerticalVelocity = Math.round((lastVelocity-currentAO.getGravityValue(currentAltitude)) * Math.cos(rad2deg));
        if(currentVerticalVelocity < 0) {
            currentVerticalVelocity = 0;
        }

        if(currentVerticalVelocity == 0) {
            currentHorizontalVelocity = currentVelocity;
        } else {
            currentHorizontalVelocity = Math.round(lastVelocity * Math.sin(rad2deg));
        }

    } else {
        currentVerticalVelocity = currentVelocity;
    }

    currentDistance = currentVelocity;

    if(currentVelocity > 0) {
        ascendingTime += timeTick;
    } else {
        ascendingTime = -1;
    }

    if(pitchProgram.running) {
        pitchProgram.running_time += timeTick;
        currentPitch += getPitchStep()/(timeMod);
        updateHUD('voyager7_pitch', Math.round(currentPitch)+'° / '+destPitch+'°');
    }

    if(rollProgram.running) {
        currentRoll += getRollStep()/(timeMod);
        updateHUD('voyager7_rollAzimuth', Math.round(90-currentRoll)+'° / '+destRoll+'°');
    }

    if(yawProgram.running) {
        currentYaw += getYawStep()/(timeMod);
        updateHUD('voyager7_yaw', Math.round(currentYaw*10)/10+'°')
    }

    if(horizontalAltitudeProgram.running) {
        horizontalAltitudeProgram.running_time += timeTick;

        var currentDuration = horizontalAltitudeProgram.steps[horizontalAltitudeProgram.currentStep].duration;
        var timeLeft = Math.round(currentDuration - horizontalAltitudeProgram.running_time);

        timeLeft = (timeLeft > 0 ? timeLeft : 0);
        if(timeLeft == 0) {
            horizontalAltitudeEstablished = 1;
        }
    }

    if(currentAltitude >= Math.round(currentAO.getCurrentDestinationAltitude())) {
        if(!stableOrbitAchievedInfo) {
            if(currentVelocity >= currentAO.getOrbitalSpeed(currentAltitude)) {
                stableOrbitAchievedInfo = true;
                if(currentPitch > 90) {
                    currentPitch = 90;
                }
                updateInformation('STABLE ORBIT ACHIEVED');
                if(autoPilotEnabled) {
                    autoPilot(missionTime);
                }
            }
        }
    }

    if(currentDistance > 0) {
        if(currentPitch <= 92) /*TODO delete*/{
            var mod_ = (100-currentPitch)/100;
            if(currentSystemStage.id == 1) {
                currentAltitude += (((currentDistance)*mod_)/(timeMod));
            } else {
                mod_ = (88.95-currentPitch)/91.05;
                currentAltitude += ((currentVerticalVelocity)*mod_)/(timeMod);
            }
        }
    }

    if(currentDistance < 0 && currentAcceleration < 0 && mainEngine.getThrust() < 100 && !maxQAchieved) {
        currentAltitude += currentDistance/timeMod;
    }

    lastVelocity = currentVelocity;

    if(currentAltitude > 0) {
        totalDistance += Math.abs(currentDistance/(timeMod));
    } else {
        lastVelocity = 0;
    }

    if(currentAltitude < 0) {
        currentAltitude = 0;
    }

    if(missionTime > 0 && lastTime != Math.round(missionTime)) {
        if(doMissionLog) {
            missionLog += (secondsToHms(missionTime)+'     '+Math.round(lastVelocity)+'    '+Math.round(currentAcceleration*10)/10)+'\n';//       '+Math.round(currentAO.getOrbitalSpeed(currentAltitude))+':'+Math.round(lastVelocity)+'\n';
        }
        lastTime = Math.round(missionTime);
        timeBasedChange(lastTime);
    }

    var viewAcceleration = currentAcceleration ;//+ (missionTime > 0 ? 9.8 : 0);

    updateHUD('voyager7_mission_time', secondsToHms(missionTime));
    updateHUD('voyager7_velocity', Math.round(lastVelocity) + ' M/S');
    updateHUD('voyager7_verticalVelocity', Math.round(currentVerticalVelocity) + ' M/S');
    updateHUD('voyager7_horizontalVelocity', Math.round(currentHorizontalVelocity) + ' M/S');
    updateHUD('voyager7_totalMass', Math.round(totalMass) + ' KG');
    updateHUD('voyager7_acceleration', Math.round(viewAcceleration*10)/10 + ' M/S');
    updateHUD('voyager7_gForce', Math.abs(Math.round(viewAcceleration/10)) + ' G');
    updateHUD('voyager7_dynamicPressure', Math.round(lastDP)+' KG/M²');
    if(currentSystemStage.id != -1) {
        updateHUD('voyager7_s'+currentSystemStage.id+'_fuel', Math.round(currentFuelMass)+' KG');
        updateHUD('voyager7_s'+currentSystemStage.id+'_thrust', Math.round(thrustNewtons)+' N');
        updateHUD('voyager7_s'+currentSystemStage.id+'_burnTime', currentSystemStage.getBurnTime(currentEpoch)+' S');
    }
    if(currentAltitude <= 1000) {
        updateHUD('voyager7_altitude', Math.round(currentAltitude) + ' M');
    } else if(currentAltitude > 1000) {
        updateHUD('voyager7_altitude', Math.round(currentAltitude/100)/10 + ' KM');
    }

    if(totalDistance <= 1000) {
        updateHUD('voyager7_distance', Math.round(totalDistance) + ' M');
    } else {
        updateHUD('voyager7_distance', Math.round(totalDistance)/1000 + ' KM');
    }
    updateHUD('voyager7_thrust', currentThrust+' %');
    updateHUD('voyager7_horizontalAltitude', getHorizontalAltitudeStatus());

    setTimeout("computeAll()", timeInterval);

}

function saveState() {
    actualState = {
        mission_time : missionTime,
        ascending_time : ascendingTime,
        main_engine : mainEngine,
        system_aps : systemAPS,
        last_time : lastTime,
        q_zero : qZero,
        stable_orbit_info : stableOrbitAchievedInfo,

        messages : {
            tower_clear : towerClearInfo,
            lift_off : liftOffInfo
        },

        velocity : {
            velocity : lastVelocity,
            horizontal : currentHorizontalVelocity,
            vertical : currentVerticalVelocity,
            acceleration : currentAcceleration,
            thrust_percent : currentThrust,
            max_q : {
                value : lastDP,
                achieved : maxQAchieved
            }
        },

        vehicle : {
            main_engine_on : mainEngine.engaged,
            total_mass : totalMass,
            pitch : {
                value : currentPitch,
                program : pitchProgram
            },
            roll : {
                value : currentRoll,
                program : rollProgram
            },
            yaw : {
                value: currentYaw,
                program: yawProgram
            },
            horizontal_altitude : {
                program : horizontalAltitudeProgram,
                established : horizontalAltitudeEstablished
            },
            let_ready : launchEscapeTowerReady
        },

        distance : {
            total : totalDistance,
            altitude : currentAltitude,
            current_astronomical_object : currentAO
        },

        saturn_v : {
            current_stage : currentSystemStage,
            S_IC : systemS1,
            S_II : systemS2,
            S_IVB : systemS3
        }
    };

    console.log(json2text(actualState));
}

function loadState() {

    for(key in actualState.main_engine) {
        mainEngine[key] = actualState.main_engine[key];
    }
    for(key in actualState.system_aps) {
        systemAPS[key] = actualState.system_aps[key];
    }
    missionTime = actualState.mission_time;
    ascendingTime = actualState.ascending_time;
    lastTime = actualState.last_time;
    qZero = actualState.q_zero;
    stableOrbitAchievedInfo = actualState.stable_orbit_info;

    towerClearInfo = actualState.messages.tower_clear;
    liftOffInfo = actualState.messages.lift_off;

    lastVelocity = actualState.velocity.velocity;
    currentHorizontalVelocity = actualState.velocity.horizontal;
    currentVerticalVelocity = actualState.velocity.vertical;
    currentAcceleration = actualState.velocity.acceleration;
    currentThrust = actualState.velocity.thrust_percent;
    mainEngine.setThrust(currentThrust);
    lastDP = actualState.velocity.max_q.value;
    maxQAchieved = actualState.velocity.max_q.achieved;

    mainEngine.engaged = actualState.vehicle.main_engine_on;
    totalMass = actualState.vehicle.total_mass;

    currentPitch = actualState.vehicle.pitch.value;
    for(key in actualState.vehicle.pitch.program) {
        pitchProgram[key] = actualState.vehicle.pitch.program[key];
    }
    currentRoll = actualState.vehicle.roll.value;
    for(key in actualState.vehicle.roll.program) {
        rollProgram[key] = actualState.vehicle.roll.program[key];
    }
    currentYaw = actualState.vehicle.yaw.value;
    for(key in actualState.vehicle.yaw.program) {
        yawProgram[key] = actualState.vehicle.yaw.program[key];
    }
    for(key in actualState.vehicle.horizontal_altitude.program) {
        horizontalAltitudeProgram[key] = actualState.vehicle.horizontal_altitude.program[key];
    }
    horizontalAltitudeEstablished = actualState.vehicle.horizontal_altitude.established;

    launchEscapeTowerReady = actualState.vehicle.let_ready;

    totalDistance = actualState.distance.total;
    currentAltitude = actualState.distance.altitude;

    for(key in actualState.distance.current_astronomical_object) {
        currentAO[key] = actualState.distance.current_astronomical_object[key];
    }

    systemS1 = new _external_tank();
    for(key in actualState.saturn_v.S_IC) {
        systemS1[key] = actualState.saturn_v.S_IC[key];
    }

    systemS2 = new _external_tank();
    for(key in actualState.saturn_v.S_II) {
        systemS2[key] = actualState.saturn_v.S_II[key];
    }

    systemS3 = new _external_tank();
    for(key in actualState.saturn_v.S_IVB) {
        systemS3[key] = actualState.saturn_v.S_IVB[key];
    }

    switch(actualState.saturn_v.current_stage.id) {
        case 1 :currentSystemStage = systemS1;break;
        case 2 :currentSystemStage = systemS2;break;
        case 3 :currentSystemStage = systemS3;break;
    }

    updateView();
    computeAll();
}


function parseRemoteData(data) {
    /*
    "current_fuel_mass":	1788934,
		"total_mass":	2579669,
		"thrust_newtons":	35255446.700001,
		"current_acceleration":	3.848696,
		"current_distance":	66.316403,
		"current_velocity":	66.316403,
		"current_vertical_velocity":	66.316403,
		"current_horizontal_velocity":	0,
		"current_fuel_burn":	1323.200000,
		"current_thrust":	100,
		"current_altitude":	669.892205,
		"total_distance":	563,
		"last_velocity":	66.316403,
		"mission_time":	22.200000,
		"ascending_time":	22.500000,
		"max_q_achieved":	0,
		"current_dynamic_pressure":	867,
		"stable_orbit_achieved":	0,
		"pitch":	0,
		"roll":	9.118000,
		"yaw":	0
     */
    
    current_fuel_mass = data.current_fuel_mass;
}