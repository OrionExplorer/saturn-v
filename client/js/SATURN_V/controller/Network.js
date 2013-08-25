JSMVC.define('SATURN_V.controller.Network', {
	name : 'Network',
	Socket : null,

	init : function() {
		var FRONTEND = SATURN_V.utils.Frontend,
			SHARED = SATURN_V.utils.Shared;

		FRONTEND.setAllButtonsDisabled(true);
		this.addLoginFormEvents();

		if( SHARED.checkBrowserForHTML5() ) {
			this.getSaturnVRemoteAddress();
		} else {
			FRONTEND.updateInformation('Error: Please check your browser for HTML5 support');
		}
	},

	addLoginFormEvents : function() {
		var me = this,
			usernameField = document.getElementById('usernameField'),
			passwordField = document.getElementById('passwordField');

		usernameField.addEventListener('keypress', function(evt) {
			me.performUserLogin(evt);
		});
		passwordField.addEventListener('keypress', function(evt) {
			me.performUserLogin(evt);
		});
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

		if(key == 13 || event.type == 'click') {
			if( userLogin.value.length == 0 || userPassword.value.length == 0 ) {
				alert('Please enter username and token!');
			} else if(userLogin.value.length > 0 & userPassword.value.length > 0) {
				me.sendLoginData(userLogin.value, userPassword.value);
			}
		}
	},

	getSaturnVRemoteAddress: function(override) {
		var me = this,
			saturnV_address = undefined,
			tmp_address = localStorage.getItem('saturnVcomputer');

		if(override == undefined) {
			saturnV_address = tmp_address;
		}

		if(saturnV_address) {
			me.connectToSaturnV(saturnV_address);
		} else {
			saturnV_address = prompt('Please enter Saturn V Main Computer IP address with port number', tmp_address);
			if (saturnV_address != null && saturnV_address != '' && saturnV_address != tmp_address) {
				localStorage.setItem('saturnVcomputer', saturnV_address);
				me.connectToSaturnV(saturnV_address);
			} else {
				if(override == undefined) {
					SATURN_V.utils.Frontend.updateInformation('Error: Unable to connect to Saturn V Main Computer');
				}
			}
		}
	},


	initSocket : function(address) {
		var me = this;

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
			var json = null,
				currentTime = document.getElementById('voyager7_mission_time').innerHTML,
				loadMask = document.getElementById('loadMask'),
				loginForm = document.getElementById('loginForm');

			try {
				json = JSON.parse(evt.data);
			} catch(ex) {
				SATURN_V.utils.Frontend.updateInformation('Error: Unable to read received data.');
				console.log(ex);
				return;
			}
			
			if(json.data_type == 'telemetry') {
				if(json.success) {
					SATURN_V.controller.MainView.parseRemoteData(json.data);
				} else {
					
				}
			} else if(json.data_type == 'command_response') {
				if(json.success) {
					if(json.msg == 'user_authorized') {
						loadMask.style.display = 'none';
						loginForm.style.display = 'none';
						document.getElementById('mainTerminal').style.display = 'block';
						SATURN_V.utils.Frontend.updateInformation('Access to remote computer granted');
						SATURN_V.utils.Frontend.setAllButtonsDisabled(false);
						/*SATURN_V.controller.Network*/me.sendCommand('', 'data', 'live');
					}
				} else {
					if(json.msg == 'authorization_required') {
						SATURN_V.utils.Frontend.updateInformation('Remote computer requires authorization');
						document.getElementById('mainTerminal').style.display = 'none';
						var loadMaskStyleDisplay = loadMask.style.display;
						if(loadMaskStyleDisplay == 'none' || loadMaskStyleDisplay == '') {
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

		this.initSocket(address);
		
	}
});