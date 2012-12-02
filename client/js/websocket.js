var Socket = undefined;

function performUserLogin(event) {
	var key = event.keyCode || event.which;
	var userLogin = document.getElementById('usernameField');
	var userPassword = document.getElementById('passwordField');

	document.getElementById('incorrectDataInfo').style.display = 'none';

	if(key == 13) {
		if( userLogin.value.length == 0 || userPassword.value.length == 0 ) {
			alert('Please enter username and token!');
		} else if(userLogin.value.length > 0 & userPassword.value.length > 0) {
			sendLoginData(userLogin.value, userPassword.value);
		}
	}
}

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
		var currentTime = document.getElementById('voyager7_mission_time').innerHTML;
		try {
			json = JSON.parse(evt.data, reviewer);
		} catch(ex) {
			updateInformation('Error: Unable to read received data.');
			console.log(ex);
			return;
		}
		
		if(json.data_type == 'telemetry') {
			if(json.success) {
				//console.log('Received telemetry data.', json.data );
				parseRemoteData(json.data);
			} else {
				
			}
		} else if(json.data_type == 'command_response') {
			var loadMask = document.getElementById('loadMask');
			var loginForm = document.getElementById('loginForm');

			if(json.success) {
				if(json.msg == 'user_authorized') {
					loadMask.style.display = 'none';
					loginForm.style.display = 'none';
					updateInformation('Access to remote computer granted');
					setAllButtonsDisabled(false);
					sendCommand('', 'data', 'live');
				}
			} else {
				if(json.msg == 'authorization_required') {
					updateInformation('Remote computer requires authorization');
					
					if(loadMask.style.display == 'none') {
						loadMask.style.display = 'block';
						loadMask.style.height = '100%';
						loginForm.style.display = 'block';
						document.getElementById('usernameField').focus();
					} else {
						document.getElementById('incorrectDataInfo').style.display = 'block';
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

function sendLoginData(username, password) {
	var sendData = {
		command_type : 'authorization',
	};

	if(username) {
		sendData['username'] = username;
	}
	if(password) {
		sendData['password'] = password;
	}

	var sendDataStr = JSON.stringify(sendData);
	Socket.send(sendDataStr);
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
	Socket.send(sendDataStr);
}