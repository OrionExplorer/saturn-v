var Socket = undefined;

function connectToVoyager7(address) {
    Socket = new WebSocket(address);
    
    Socket.onclose = function() {
        updateInformation('Connection with Voyager 7 Main Computer is closed');
        setAllButtonsDisabled(true);
    }
    Socket.onopen = function() {
        updateInformation('Connection with Voyager 7 Main Computer opened succesfully');
        sendCommand('', 'authorization');
    }

    Socket.onmessage = function(evt) {
        var json = null;
        try {
            json = eval('(' + evt.data + ')');
        } catch(ex) {
            updateInformation('Error: Unable to read received data.');
            console.log(ex);
            return;
        }
        
        if(json.data_type == 'telemetry') {
            if(json.success) {
                console.log('Received telemetry data.', json.data );
                parseRemoteData(json.data);
            } else {
                
            }
        } else if(json.data_type == 'command_response') {
            if(json.success) {
                if(json.msg == 'user_authorized') {
                    updateInformation('Access to remote computer granted');
                    setAllButtonsDisabled(false);
                    sendCommand('', 'data', 'live');
                } else {
                    updateInformation(json.msg);
                }
            } else {
                if(json.msg == 'authorization_required') {
                    updateInformation('Remote computer requires authorization');
                    var pass = '';
                    pass = prompt('Please enter authorization key', '');
                    if(pass != null && pass != '') {
                        sendCommand(pass, 'authorization');
                    }
                } else {
                    updateInformation(json.msg);
                }
            }
        }
    }
}

function getVoyager7RemoteAddress() {
    var voyager7_address = localStorage.getItem('voyager7computer');
    if(voyager7_address) {
        connectToVoyager7(voyager7_address);
    } else {
        voyager7_address = prompt('Please enter Voyager 7 Main Computer IP address with port number', 'ws://');
        if (voyager7_address != null && voyager7_address != '') {
            localStorage.setItem('voyager7computer', voyager7_address);
            connectToVoyager7(voyager7_address);
        } else {
            updateInformation('Error: Unable to connect to Voyager 7 Main Computer');
        }
    }
}

function checkBrowserForHTML5() {
    return ('WebSocket' in window && 'localStorage' in window);
}