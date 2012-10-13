function kotlecikButtonController(id) {
    var button = document.getElementById(id);
    Socket.send('konsumpcja kotlecika!');
    
}
function horizontalAltitudeButtonController(id) {
    var button = document.getElementById(id);
    switch(document.getElementById(id).value) {
        case 'START' : {
                disableEl(id);
                if(execCommand('HORIZONTAL_ALTITUDE_PROGRAM', 'START')) {
                    enableEl(id);
                    updateHUD(id, horizontalAltitudeProgram.getPossibleAction());//, horizontalAltitudeProgram.steps[horizontalAltitudeProgram.currentStep].duration*1000, 'enableEl("'+id+'")');
                } else {
                    enableEl(id);
                }
        } break;
        case 'STOP' : {
                if(execCommand('HORIZONTAL_ALTITUDE_PROGRAM', 'STOP')) {
                    updateHUD(id, horizontalAltitudeProgram.getPossibleAction());
                }
        } break;
    }
}

function pitchRollYawButtonController(id) {
    var button = document.getElementById(id);
    switch(button.id) {
        case 'pitchButton' : {
                if(execCommand('PITCH_ROLL_PROGRAM', button.value, 'PITCH')) {
                    updateHUD(id, pitchProgram.getPossibleAction());
                }
        } break;
        case 'rollButton' : {
                if(execCommand('PITCH_ROLL_PROGRAM', button.value, 'ROLL')) {
                    updateHUD(id, rollProgram.getPossibleAction());
                }
        } break;
        case 'yawButton' : {
                if(execCommand('YAW_PROGRAM', button.value, 'YAW')) {
                    updateHUD(id, yawProgram.getPossibleAction());
                }
        } break;
    }
}
/**
 * mainEngineEngageButtonController
 * @id - DOM element id to update
 */
function mainEngineEngageButtonController(id) {
    switch(document.getElementById(id).value) {
        case 'ENGAGE' : {
                updateHUD(id, 'ENGAGING...');
                disableEl(id);
                if(execCommand('MAIN_ENGINE', 'START')) {
                    updateHUD(id, 'DISENGAGE', 2000, 'enableEl("'+id+'")');
                } else {
                    updateHUD(id, 'ENGAGE');
                    enableEl(id);
                }
                
        }break;
        case 'DISENGAGE' : {
                updateHUD(id, 'DISENGAGING...');
                disableEl(id);
                if(execCommand('MAIN_ENGINE', 'STOP')) {
                    updateHUD(id, 'ENGAGE', 2000, 'enableEl("'+id+'")');
                } else {
                    updateHUD(id, 'DISENGAGE');
                    enableEl(id);
                }
        }break;
    }

    updateHUD('voyager7_mainEngine', mainEngine.getEngagedStatus(), 3000);

}

function systemMPSButtonController(id) {
    var button = document.getElementById(id);
    switch(button.id) {
        case 'systemAPSButton' : {
                if(button.value == 'ENGAGE') {
                    updateHUD(id, 'ENGAGING...');
                    disableEl(id);
                    if(execCommand('APS', 'START')) {
                        updateHUD(id, 'DISENGAGE', 2000, 'enableEl("'+id+'")');
                    } else {
                        updateHUD(id, 'ENGAGE');
                        enableEl(id);
                    }
                } else {
                    updateHUD(id, 'DISENGAGING...');
                    disableEl(id);
                    if(execCommand('APS', 'STOP')) {
                        updateHUD(id, 'ENGAGE', 2000, 'enableEl("'+id+'")');
                    } else {
                        updateHUD(id, 'DISENGAGE');
                        enableEl(id);
                    }
                }

                //updateHUD('voyager7_systemAPS', systemAPS.getEngagedStatus(), 3000);
        } break;
        case 'systemDPSButton' : {
                if(button.value == 'ENGAGE') {
                    updateHUD(id, 'ENGAGING...');
                    disableEl(id);
                    if(execCommand('DPS', 'START')) {
                        updateHUD(id, 'DISENGAGE', 2000, 'enableEl("'+id+'")');
                    } else {
                        updateHUD(id, 'ENGAGE');
                        enableEl(id);
                    }
                } else {
                    updateHUD(id, 'DISENGAGING...');
                    disableEl(id);
                    if(execCommand('DPS', 'STOP')) {
                        updateHUD(id, 'ENGAGE', 2000, 'enableEl("'+id+'")');
                    } else {
                        updateHUD(id, 'DISENGAGE');
                        enableEl(id);
                    }
                }

                //updateHUD('voyager7_systemDPS', systemDPS.getEngagedStatus(), 3000);
        } break;
    }
}

function mainEngineThrustButtonController(id) {
    switch(document.getElementById(id).value) {
        case 'UP' : {
                execCommand('THRUST', 'INCREASE', 5, false);
        } break;
        case 'DOWN' : {
                execCommand('THRUST', 'DECREASE', 5, false);
        } break;
        case 'FULL' : {
                execCommand('THRUST', 'FULL', null, false);
        } break;
        case 'NULL' : {
                execCommand('THRUST', 'NULL', null, false);
        } break;
    }
}

function towerJettisonButtonController(id) {
    var button = document.getElementById(id);
    switch(button.id) {
        case 'towerJettisonButton' : {
            if(execCommand('LET', 'JETTISON')) {
                disableEl(id);
            }
        } break;
    }
}

function antennasButtonController(id) {
    var button = document.getElementById(id);
    switch(button.id) {
        case 'antennaHGButton' : {
            if(button.value == 'ENGAGE') {
                updateHUD(id, 'ENGAGING...');
                disableEl(id);
                if(execCommand('HG_ANTENNA', 'ACTIVATE')) {
                    updateHUD(id, 'DISENGAGE', 15000, 'enableEl("'+id+'")');
                } else {
                    updateHUD(id, 'ENGAGE');
                    enableEl(id);
                }
            } else {
                updateHUD(id, 'DISENGAGING...');
                disableEl(id);
                if(execCommand('HG_ANTENNA', 'DEACTIVATE')) {
                    updateHUD(id, 'ENGAGE', 15000, 'enableEl("'+id+'")');
                } else {
                    updateHUD(id, 'DISENAGE');
                    enableEl(id);
                }
            }

            updateHUD('voyager7_highGainAntenna', highGainAntenna.getEngagedStatus(), 15000);
        } break;
        case 'antennaLGButton' : {
            if(button.value == 'ENGAGE') {
                updateHUD(id, 'ENGAGING...');
                disableEl(id);
                if(execCommand('LG_ANTENNA', 'ACTIVATE')) {
                    updateHUD(id, 'DISENGAGE', 5000, 'enableEl("'+id+'")');
                } else {
                    updateHUD(id, 'ENGAGE');
                    enableEl(id);
                }
            } else {
                updateHUD(id, 'DISENGAGING...');
                disableEl(id);
                if(execCommand('LG_ANTENNA', 'DEACTIVATE')) {
                    updateHUD(id, 'ENGAGE', 5000, 'enableEl("'+id+'")');
                } else {
                    updateHUD(id, 'DISENGAGE');
                    enableEl(id);
                }
            }

            updateHUD('voyager7_lowGainAntenna', lowGainAntenna.getEngagedStatus(), 5000);
        } break;
    }
}

function systemS1ButtonController(id) {
    var button = document.getElementById(id);
    switch(button.id) {
        case 'systemS1FuelButton' : {
            if(button.value == 'TANK') {
                updateHUD(id, 'TANKING...');
                disableEl(id);
                if(execCommand('S1', 'TANK', systemS1.MAX_FUEL)) {
                    updateHUD(id, 'RELEASE', 1000, 'enableEl("'+id+'")');
                } else {
                    updateHUD(id, 'TANK');
                    enableEl(id);
                }
            } else {
                updateHUD(id, 'RELEASING...');
                disableEl(id);
                if(execCommand('S1', 'RELEASE')) {
                    updateHUD(id, 'TANK', 1000, 'enableEl("'+id+'")');
                } else {
                    updateHUD(id, 'RELEASE');
                    enableEl(id);
                }
            }
            updateHUD('voyager7_s1_fuel', systemS1.getFuel()+' KG', 1000);
        } break;
        case 'systemS1ActionButton' : {
            if(button.value == 'OUTBOARD') {
                updateHUD(id, 'STAGING...');
                disableEl(id);
                if(execCommand('S1', 'DETACH')) {
                    updateHUD(id, 'STAGED', 3000, 'disableEl("'+id+'")');
                } else {
                    updateHUD(id, 'OUTBOARD');
                    enableEl(id);
                }
            }
            updateHUD('voyager7_s1_status', systemS1.getAttached()+'', 3000);
        } break;
        case 'systemS1CenterEngineCutoffButton' : {
            if(currentSystemStage.id == 1) {
                if(execCommand('THRUST', 'DECREASE', 20, true) == true) {
                    disableEl(id);
                    updateInformation('S-IC CENTER ENGINE CUTOFF');
                }
            } else {
                updateInformation('ERROR: UNABLE TO CUTOFF S-IC CENTER ENGINE. CHECK CONFIGURATION');
            }
        } break;
    }
}

function systemS2ButtonController(id) {
    var button = document.getElementById(id);
    switch(button.id) {
        case 'systemS2FuelButton' : {
            if(button.value == 'TANK') {
                updateHUD(id, 'TANKING...');
                disableEl(id);
                if(execCommand('S2', 'TANK', systemS2.MAX_FUEL)) {
                    updateHUD(id, 'RELEASE', 1000, 'enableEl("'+id+'")');
                } else {
                    updateHUD(id, 'TANK');
                    enableEl(id);
                }
            } else {
                updateHUD(id, 'RELEASING...');
                disableEl(id);
                if(execCommand('S2', 'RELEASE')) {
                    updateHUD(id, 'TANK', 1000, 'enableEl("'+id+'")');
                } else {
                    updateHUD(id, 'RELEASE');
                    enableEl(id);
                }
            }
            updateHUD('voyager7_s2_fuel', systemS2.getFuel()+' KG', 1000);
        } break;
        case 'systemS2ActionButton' : {
            if(button.value == 'OUTBOARD') {
                updateHUD(id, 'STAGING...');
                disableEl(id);
                if(execCommand('S2', 'DETACH')) {
                    updateHUD(id, 'STAGED', 3000, 'disableEl("'+id+'")');
                } else {
                    updateHUD(id, 'OUTBOARD');
                    enableEl(id);
                }
            }
            updateHUD('voyager7_s2_status', systemS2.getAttached()+'', 3000);
        } break;
        case 'systemS2CenterEngineCutoffButton' : {
            if(currentSystemStage.id == 2) {
                if(execCommand('THRUST', 'DECREASE', 20, true) == true) {
                    disableEl(id);
                    updateInformation('S-II CENTER ENGINE CUTOFF');
                }
            } else {
                updateInformation('ERROR: UNABLE TO CUTOFF S-II CENTER ENGINE. CHECK CONFIGURATION');
            }
        } break;
    }
}

function systemS3ButtonController(id) {
    var button = document.getElementById(id);
    switch(button.id) {
        case 'systemS3FuelButton' : {
            if(button.value == 'TANK') {
                updateHUD(id, 'TANKING...');
                disableEl(id);
                if(execCommand('S3', 'TANK', systemS3.MAX_FUEL)) {
                    updateHUD(id, 'RELEASE', 1000, 'enableEl("'+id+'")');
                } else {
                    updateHUD(id, 'TANK');
                    enableEl(id);
                }
            } else {
                updateHUD(id, 'RELEASING...');
                disableEl(id);
                if(execCommand('S3', 'RELEASE')) {
                    updateHUD(id, 'TANK', 1000, 'enableEl("'+id+'")');
                } else {
                    updateHUD(id, 'RELEASE');
                    enableEl(id);
                }
            }
            updateHUD('voyager7_s3_fuel', systemS3.getFuel()+' KG', 1000);
        } break;
        case 'systemS3ActionButton' : {
            if(button.value == 'OUTBOARD') {
                updateHUD(id, 'STAGING...');
                disableEl(id);
                if(execCommand('S3', 'DETACH')) {
                    updateHUD(id, 'STAGED', 3000, 'disableEl("'+id+'")');
                } else {
                    updateHUD(id, 'OUTBOARD');
                    enableEl(id);
                }
            }
            updateHUD('voyager7_s3_status', systemS3.getAttached()+'', 3000);
        } break;
        case 'systemS3Restart' : {
            if(execCommand('S3', 'RESTART')) {
                updateHUD(id, 'RESTART', 10, 'enableEl("'+id+'")');
            }
        }
    }
}