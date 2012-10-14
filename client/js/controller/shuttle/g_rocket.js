function kotlecikButtonController(id) {
    var button = document.getElementById(id);
    Socket.send('konsumpcja kotlecika!');
    
}

function pitchRollYawButtonController(id) {
    var button = document.getElementById(id);
    switch(button.id) {
        case 'pitchButton' : {
                if(execCommand('PITCH_PROGRAM', button.value)) {
                    updateHUD(id, pitchProgram.getPossibleAction());
                }
        } break;
        case 'rollButton' : {
                if(execCommand('ROLL_PROGRAM', button.value)) {
                    updateHUD(id, rollProgram.getPossibleAction());
                }
        } break;
        case 'yawButton' : {
                if(execCommand('YAW_PROGRAM', button.value)) {
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
                if(execCommand('INTERNAL_GUIDANCE', 'START')) {
                    updateHUD(id, 'DISENGAGE', 2000, 'enableEl("'+id+'")');
                } else {
                    updateHUD(id, 'ENGAGE');
                    enableEl(id);
                }
                
        }break;
        case 'DISENGAGE' : {
                updateHUD(id, 'DISENGAGING...');
                disableEl(id);
                if(execCommand('INTERNAL_GUIDANCE', 'STOP')) {
                    updateHUD(id, 'ENGAGE', 2000, 'enableEl("'+id+'")');
                } else {
                    updateHUD(id, 'DISENGAGE');
                    enableEl(id);
                }
        }break;
    }

    //updateHUD('voyager7_mainEngine', mainEngine.getEngagedStatus(), 3000);

}

function systemMPSButtonController(id) {
    var button = document.getElementById(id);
    switch(button.id) {
        case 'systemAPSButton' : {
                if(button.value == 'ENGAGE') {
                    updateHUD(id, 'ENGAGING...');
                    disableEl(id);
                    if(execCommand('MAIN_ENGINE', 'START')) {
                        updateHUD(id, 'DISENGAGE', 2000, 'enableEl("'+id+'")');
                    } else {
                        updateHUD(id, 'ENGAGE');
                        enableEl(id);
                    }
                } else {
                    updateHUD(id, 'DISENGAGING...');
                    disableEl(id);
                    if(execCommand('MAIN_ENGINE', 'STOP')) {
                        updateHUD(id, 'ENGAGE', 2000, 'enableEl("'+id+'")');
                    } else {
                        updateHUD(id, 'DISENGAGE');
                        enableEl(id);
                    }
                }
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
                execCommand('THRUST', 'FULL_THRUST', null, false);
        } break;
        case 'NULL' : {
                execCommand('THRUST', 'NULL_THRUST', null, false);
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
            if(execCommand('S1', 'CENTER_ENGINE_CUTOFF', 20, true) == true) {
                disableEl(id);
            }
        } break;
    }
}

function systemS2ButtonController(id) {
    var button = document.getElementById(id);
    switch(button.id) {
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
            if(execCommand('S2', 'CENTER_ENGINE_CUTOFF') == true) {
                disableEl(id);
                updateInformation('S-II CENTER ENGINE CUTOFF');
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