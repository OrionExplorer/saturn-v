function appInit() {
    startTag('div id="terminal"');
        startTag('div class="tooltip"');
            showTitle();
            showTimer();
            setTimeout("updateTime()", 1000);
        endTag('div');

        showInformation('idle');

            startTag('div id="reports-1-panel"');
                showVelocityData();
                showEngines();
                showPitchRollYaw();
                showDistance();
            endTag('div');

            startTag('div id="reports-2-panel"');
                showS3Component();
                showS2Component();
                showS1Component();
            endTag('div');

            startTag('div id="control-1-unit"');
                controlPitchRollYawProgram();
                controlEngines();
                controlDevices();
            endTag('div');

            startTag('div id="control-2-unit"');
                controlS3Component();
                controlS2Component();
                controlS1Component();
            endTag('div');
    endTag('div');
    setAllButtonsDisabled(true);
}
appInit();
disableThrustChange();

function sendCommand( command, commandType, responseMode ) {
    var sendData = {};
    
    if(command) {
        sendData['command'] = command;
    }
    if(commandType) {
        sendData['command_type'] = commandType;
    }
    if(responseMode) {
        sendData['response_mode'] = responseMode;
    }
    
    var sendDataStr = JSON.stringify(sendData);
    
    console.log('sendDataStr = '+sendDataStr);
    
    Socket.send(sendDataStr);
}
if( checkBrowserForHTML5() ) {
    getVoyager7RemoteAddress();
} else {
    updateInformation('Error: Please check your browser for HTML5 support');
}