JSMVC.define('SATURN_V.controller.Network', {
	views : [],
	models : [],
	name : 'Network',
	Socket : null,

	init : function() {
		var FRONTEND = SATURN_V.utils.Frontend;
		var SHARED = SATURN_V.utils.Shared;

		FRONTEND.setAllButtonsDisabled(true);
		SATURN_V.controller.Network.addLoginFormEvents();

		if( SHARED.checkBrowserForHTML5() ) {
			SATURN_V.controller.Network.getSaturnVRemoteAddress();
		} else {
			FRONTEND.updateInformation('Error: Please check your browser for HTML5 support');
		}
	},

	addLoginFormEvents : function() {
		var me = this,
			usernameField = document.getElementById('usernameField'),
			passwordField = document.getElementById('passwordField');

		usernameField.addEventListener('keypress', SATURN_V.controller.Network.performUserLogin);
		passwordField.addEventListener('keypress', SATURN_V.controller.Network.performUserLogin);
	},

	sendCommand : function(command, commandType, responseMode) {
		var sendDataStr = '',
			sendData = {};
		
		if(command) {
			sendData['command'] = command;
		}
		if(commandType) {
			sendData['command_type'] = commandType;
		}
		if(responseMode) {
			sendData['response_mode'] = responseMode;
		}
		
		sendDataStr = JSON.stringify(sendData);	
		this.Socket.send(sendDataStr);
	},

	sendLoginData : function(username, password) {
		var sendDataStr = '',
			sendData = {
				command_type : 'authorization',
			};

		if(username) {
			sendData['username'] = username;
		}
		if(password) {
			sendData['password'] = password;
		}

		sendDataStr = JSON.stringify(sendData);
		this.Socket.send(sendDataStr);
	},

	performUserLogin : function(event) {
		var key = event.keyCode || event.which,
			userLogin = document.getElementById('usernameField'),
			userPassword = document.getElementById('passwordField'),
			me = this;

		document.getElementById('incorrectDataInfo').style.display = 'none';

		if(key == 13) {
			if( userLogin.value.length == 0 || userPassword.value.length == 0 ) {
				alert('Please enter username and token!');
			} else if(userLogin.value.length > 0 & userPassword.value.length > 0) {
				SATURN_V.controller.sendLoginData(userLogin.value, userPassword.value);
			}
		}
	},

	getSaturnVRemoteAddress: function(override) {
		var me = this,
			saturnV_address = undefined;

		if(override == undefined) {
			saturnV_address = localStorage.getItem('saturnVcomputer');
		}

		if(saturnV_address) {
			me.connectToSaturnV(saturnV_address);
		} else {
			saturnV_address = prompt('Please enter Saturn V Main Computer IP address with port number', 'ws://');
			if (saturnV_address != null && saturnV_address != '') {
				localStorage.setItem('saturnVcomputer', saturnV_address);
				SATURN_V.controller.Network.Network.connectToSaturnV(saturnV_address);
			} else {
				if(override == undefined) {
					SATURN_V.utils.Frontend.updateInformation('Error: Unable to connect to Saturn V Main Computer');
				}
			}
		}
	},

	connectToSaturnV : function(address) {
		var me = this;

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

		if(this.Socket) {
			this.Socket.close();
		}

		SATURN_V.utils.Frontend.updateInformation('Connection with Saturn V Main Computer is being established...');
		this.Socket = new WebSocket(address);
		
		this.Socket.onclose = function() {
			SATURN_V.utils.Frontend.updateInformation('Connection with Saturn V Main Computer is closed');
			SATURN_V.utils.Frontend.setAllButtonsDisabled(true);
		}
		this.Socket.onopen = function() {
			SATURN_V.utils.Frontend.updateInformation('Connection with Saturn V Main Computer opened succesfully');
			me.sendCommand('', 'authorization');
		}

		this.Socket.onmessage = function(evt) {
			var json = null;
			var currentTime = document.getElementById('voyager7_mission_time').innerHTML;
			try {
				json = JSON.parse(evt.data, reviewer);
			} catch(ex) {
				SATURN_V.utils.Frontend.updateInformation('Error: Unable to read received data.');
				console.log(ex);
				return;
			}
			
			if(json.data_type == 'telemetry') {
				if(json.success) {
					SATURN_V.controller.ControlPanel.parseRemoteData(json.data);
				} else {
					
				}
			} else if(json.data_type == 'command_response') {
				var loadMask = document.getElementById('loadMask');
				var loginForm = document.getElementById('loginForm');

				if(json.success) {
					if(json.msg == 'user_authorized') {
						loadMask.style.display = 'none';
						loginForm.style.display = 'none';
						SATURN_V.utils.Frontend.updateInformation('Access to remote computer granted');
						SATURN_V.utils.Frontend.setAllButtonsDisabled(false);
						SATURN_V.controller.Network.sendCommand('', 'data', 'live');
					}
				} else {
					if(json.msg == 'authorization_required') {
						SATURN_V.utils.Frontend.updateInformation('Remote computer requires authorization');
						
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
				SATURN_V.utils.Frontend.updateInformation('<'+currentTime+'> '+json.data.user+': '+json.data.text);
			} else if(json.data_type == 'new_user') {
				SATURN_V.utils.Frontend.updateInformation('<'+currentTime+'> New user: '+ json.msg);
			} else if(json.data_type == 'del_user') {
				SATURN_V.utils.Frontend.updateInformation('<'+currentTime+'> User left: '+ json.msg);
			}
		}
	}
});