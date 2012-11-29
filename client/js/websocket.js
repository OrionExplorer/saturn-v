var Socket = undefined;

function connectToVoyager7(address) {
	/*
	Funkcja pomocnicza do metody JSON.parse
	*/
	function reviewer(key, value) {
		var type;
		if (value && typeof value === 'object') {
			type = value.type;
			if (typeof type === 'string' && typeof window[type] === 'function') {
				return new (window[type])(value);
			}
		}
		return value;
	}

	Socket = new WebSocket(address);
	
	Socket.onclose = function() {
		updateInformation('Connection with Saturn V Main Computer is closed');
		setAllButtonsDisabled(true);
	}
	Socket.onopen = function() {
		updateInformation('Connection with Saturn V Main Computer opened succesfully');
		sendCommand('', 'authorization');
	}

	Socket.onmessage = function(evt) {
		var json = null;
		var currentTime = document.getElementById('voyager7_timeInfo').innerHTML;
		try {
			json = JSON.parse(evt.data, reviewer);//eval('(' + evt.data + ')');
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
				}
			} else {
				if(json.msg == 'authorization_required') {
					updateInformation('Remote computer requires authorization');
					var username = '';
					username = prompt('Please enter your name', '');
					if(username != null && username != '') {
						var pass = '';
						pass = prompt('Please enter authorization token', '');
						if(pass != null && pass != '') {
							sendCommand(username, 'username');	
							sendCommand(pass, 'authorization');
						}
					}
				}
			}
		} else if(json.data_type == 'chat_message') {
			updateInformation('<'+currentTime+'> '+json.data.user+': '+json.data.text);
		} else if(json.data_type == 'new_user') {
			updateInformation('<'+currentTime+'> New user: '+ json.msg);
		} else if(json.data_type == 'del_user') {
			updateInformation('<'+currentTime+'> User left: '+ json.msg);
		}
	}
}

function getVoyager7RemoteAddress() {
	var voyager7_address = localStorage.getItem('voyager7computer');
	if(voyager7_address) {
		connectToVoyager7(voyager7_address);
	} else {
		voyager7_address = prompt('Please enter Saturn V Main Computer IP address with port number', 'ws://');
		if (voyager7_address != null && voyager7_address != '') {
			localStorage.setItem('voyager7computer', voyager7_address);
			connectToVoyager7(voyager7_address);
		} else {
			updateInformation('Error: Unable to connect to Saturn V Main Computer');
		}
	}
}

function checkBrowserForHTML5() {
	return ('WebSocket' in window && 'localStorage' in window);
}

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