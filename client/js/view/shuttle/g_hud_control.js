function updateHUD(id, val, delay, callback) {
    if(delay != undefined) {
        if(callback != undefined) {
            setTimeout('updateEl(\''+id+'\', \''+val+'\', \''+callback+'\')', delay);
        } else {
            setTimeout('updateEl(\''+id+'\', \''+val+'\')', delay);
        }
    } else {
        updateEl(id, val);
    }
}

function disableThrustChange() {
    disableEl('mainEngineThrustFullButton');
    disableEl('mainEngineThrustUpButton');
    disableEl('mainEngineThrustDownButton');
    disableEl('mainEngineThrustNullButton');
}

function enableThrustChange() {
    enableEl('mainEngineThrustFullButton');
    enableEl('mainEngineThrustUpButton');
    enableEl('mainEngineThrustDownButton');
    enableEl('mainEngineThrustNullButton');
}

function controlEngines() {
    print('<div class="box-2-info">'+
            '<table>'+
            '<caption><strong>ENGINES CONTROL PANEL</strong></caption>'+
                _showRecord('<strong>INTERNAL GUIDANCE</strong>', showButton('ENGAGE', 'mainEngineEngageButtonController', 'mainEngineEngageButton'))
       );

    print(_showRecord('<strong>THRUST</strong>',
    showButton('FULL', 'mainEngineThrustButtonController', 'mainEngineThrustFullButton')+'</br>'+
    showButton('UP', 'mainEngineThrustButtonController', 'mainEngineThrustUpButton')+'</br>'+
    showButton('DOWN', 'mainEngineThrustButtonController', 'mainEngineThrustDownButton')+'</br>'+
    showButton('NULL', 'mainEngineThrustButtonController', 'mainEngineThrustNullButton'))+'</br>'+
    _showRecord('<strong>MAIN ENGINE</strong>', showButton('ENGAGE', 'systemMPSButtonController', 'systemAPSButton'))+
        '</table>'+
        '</div>');
}

function controlPitchRollYawProgram() {
    print('<div class="box-1-info">'+
            '<table>'+
            '<caption><strong>PITCH-ROLL PROGRAM</strong></caption>'+
            _showRecord('<strong>PITCH</strong>',
                showButton('START', 'pitchRollYawButtonController', 'pitchButton'))+
            _showRecord('<strong>ROLL</strong>',
                showButton('START', 'pitchRollYawButtonController', 'rollButton'))+
            _showRecord('<strong>YAW</strong>',
                showButton('START', 'pitchRollYawButtonController', 'yawButton'))+
        '</table>'+
        '</div>');
}

function controlS1Component() {
    print('<div class="box-1-info">'+
            '<table>'+
            '<caption><strong>S-IC STAGE CONTROL PANEL</strong></caption>'+
            _showRecord('<strong>ENGINE CUTOFF</strong>',
                showButton('CENTER', 'systemS1ButtonController', 'systemS1CenterEngineCutoffButton')+'</br>'+
                showButton('OUTBOARD', 'systemS1ButtonController', 'systemS1ActionButton'))+
//            _showRecord('<strong>CENTER ENGINE</br>CUTOFF</strong>',
//                showButton('ENGAGE', 'systemS1ButtonController', 'systemS1CenterEngineCutoffButton'))+
//            _showRecord('<strong>OUTBOARD ENGINE</br>CUTOFF</strong>',
//                showButton(systemS1.getPossibleAction(), 'systemS1ButtonController', 'systemS1ActionButton'))+
        '</table>'+
        '</div>');
}

function controlS2Component() {
    print('<div class="box-1-info">'+
            '<table>'+
            '<caption><strong>S-II STAGE CONTROL PANEL</strong></caption>'+
            _showRecord('<strong>ENGINE CUTOFF</strong>',
                showButton('CENTER', 'systemS2ButtonController', 'systemS2CenterEngineCutoffButton')+'</br>'+
                showButton('OUTBOARD', 'systemS2ButtonController', 'systemS2ActionButton'))+
//            _showRecord('<strong>CENTER ENGINE</br>CUTOFF</strong>',
//                showButton('ENGAGE', 'systemS2ButtonController', 'systemS2CenterEngineCutoffButton'))+
//            _showRecord('<strong>OUTBOARD ENGINE</br>CUTOFF</strong>',
//                showButton(systemS2.getPossibleAction(), 'systemS2ButtonController', 'systemS2ActionButton'))+
        '</table>'+
        '</div>');
}

function controlS3Component() {
    print('<div class="box-1-info">'+
            '<table>'+
            '<caption><strong>S-IVB STAGE CONTROL PANEL</strong></caption>'+
            _showRecord('<strong>RESTART</strong>',
                showButton('ENGAGE', 'systemS3ButtonController', 'systemS3Restart'))+
            _showRecord('<strong>ENGINE CUTOFF</strong>',
                showButton('OUTBOARD', 'systemS3ButtonController', 'systemS3ActionButton'))+
//            _showRecord('<strong>OUTBOARD ENGINE</br>CUTOFF</strong>',
//                showButton(systemS3.getPossibleAction(), 'systemS3ButtonController', 'systemS3ActionButton'))+
        '</table>'+
        '</div>');
}

function controlDevices() {
    print('<div class="box-1-info">'+
            '<table>'+
            '<caption><strong>DEVICES</strong></caption>'+
            _showRecord('<strong>LAUNCH ESCAPE TOWER</strong>',
                showButton('JETTISON', 'towerJettisonButtonController', 'towerJettisonButton')+'</br>'+
                showButton('ENGAGE', 'towerJettisonButtonController', 'towerJettisonEngageButton'))+
           _showRecord('<strong>GASTRONOMICAL ARM</strong>',
                showButton('GET KOTLECIK', 'kotlecikButtonController', 'kotlecikButton'))+
        '</table>'+
        '</div>');
}