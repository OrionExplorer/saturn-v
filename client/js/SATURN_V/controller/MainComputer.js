JSMVC.define('SATURN_V.controller.MainComputer', {
	name : 'MainComputer',
	requires : ['SATURN_V.utils.Events'],

	execCommand : function(device, command, value) {
		var commandStr = '',
			devices = {
				INTERNAL_GUIDANCE : 0,
				S1 : 1,
				S2 : 2,
				S3 : 3,
				MAIN_ENGINE : 4,
				THRUST : 5,
				PITCH_PROGRAM : 6,
				ROLL_PROGRAM : 7,
				YAW_PROGRAM : 8,
				LET : 9,
				AUTOPILOT : 10,
				HOLDDOWN_ARMS : 11,
				COUNTDOWN : 12,
				PITCH_MOD : 13,
				ROLL_MOD : 14,
				YAW_MOD : 15
			},
			commands = {
				START : 0,
				STOP : 1,
				TANK : 2,
				ATTACH : 3,
				DETACH : 4,
				CENTER_ENGINE_CUTOFF : 5,
				RESTART : 6,
				FULL_THRUST : 7,
				NULL_THRUST : 8,
				INCREASE : 9,
				DECREASE : 10,
				JETTISON : 11
			};
		
		if(SATURN_V.controller.Network.Socket) {
			commandStr = devices[device]+' '+commands[command]+' '+(value ? value : '0');
			SATURN_V.controller.Network.sendCommand(commandStr, 'computer');
		}
	},

	pitchRollYawButtonController : function(id) {
		var button = document.getElementById(id),
			buttonVal = (button.value == 'ON' ? 'START' : 'STOP');
		switch(button.id) {
			/* PITCH CONTROL */
			case 'pitchButton' : {
					this.execCommand('PITCH_PROGRAM', buttonVal);
			} break;
			case 'pitchIncButton' : {
					this.execCommand('PITCH_MOD', 'INCREASE', 1);
			} break;
			case 'pitchDecButton' : {
					this.execCommand('PITCH_MOD', 'DECREASE', -1);
			} break;
			/* ROLL CONTROL */
			case 'rollButton' : {
					this.execCommand('ROLL_PROGRAM', buttonVal);
			} break;
			case 'rollIncButton' : {
					this.execCommand('ROLL_MOD', 'INCREASE', 1);
			} break;
			case 'rollDecButton' : {
					this.execCommand('ROLL_MOD', 'DECREASE', -1);
			} break;
			/* YAW CONTROL */
			case 'yawButton' : {
					this.execCommand('YAW_PROGRAM', buttonVal);
			} break;
			case 'yawIncButton' : {
					this.execCommand('YAW_MOD', 'INCREASE', 1);
			} break;
			case 'yawDecButton' : {
					this.execCommand('YAW_MOD', 'DECREASE', -1);
			} break;
		}
	},

	mainEngineEngageButtonController : function(id) {
		switch(document.getElementById(id).value) {
			case 'ENGAGE' : {
					this.execCommand('MAIN_ENGINE', 'START');
					
			} break;
			case 'DISENGAGE' : {
					this.execCommand('MAIN_ENGINE', 'STOP');
			} break;
		}
	},

	mainEngineThrustButtonController : function(id) {
		switch(document.getElementById(id).value) {
			case '+' : {
					this.execCommand('THRUST', 'INCREASE', 5, false);
			} break;
			case '-' : {
					this.execCommand('THRUST', 'DECREASE', 5, false);
			} break;
			case 'FULL' : {
					this.execCommand('THRUST', 'FULL_THRUST', null, false);
			} break;
			case 'NULL' : {
					this.execCommand('THRUST', 'NULL_THRUST', null, false);
			} break;
		}
	},

	mainEngineButtonController : function(id) {
		var button = document.getElementById(id);
		switch(button.id) {
			case 'mainEngineButton' : {
					if(button.value == 'ENGAGE') {
						this.execCommand('MAIN_ENGINE', 'START');
					} else {
						this.execCommand('MAIN_ENGINE', 'STOP');
					}
			} break;
		}
	},

	internalGuidanceButtonController : function(id) {
		var button = document.getElementById(id);
		switch(button.id) {
			case 'internalGuidanceButton' : {
					if(button.value == 'ENGAGE') {
						this.execCommand('INTERNAL_GUIDANCE', 'START');
					} else {
						this.execCommand('INTERNAL_GUIDANCE', 'STOP');
					}
			} break;
		}
	},

	towerJettisonButtonController : function(id) {
		var button = document.getElementById(id);
		switch(button.id) {
			case 'towerJettisonButton' : {
				execCommand('LET', 'JETTISON');
			} break;
		}
	},

	systemS1ButtonController : function(id) {
		var button = document.getElementById(id);
		switch(button.id) {
			case 'systemS1ActionButton' : {
				if(button.value == 'OUTBOARD') {
					this.execCommand('S1', 'DETACH');
				}
			} break;
			case 'systemS1CenterEngineCutoffButton' : {
				this.execCommand('S1', 'CENTER_ENGINE_CUTOFF', 20, true)
			} break;
		}
	},

	systemS2ButtonController : function(id) {
		var button = document.getElementById(id);
		switch(button.id) {
			case 'systemS2ActionButton' : {
				if(button.value == 'OUTBOARD') {
					this.execCommand('S2', 'DETACH');
				}
			} break;
			case 'systemS2CenterEngineCutoffButton' : {
				this.execCommand('S2', 'CENTER_ENGINE_CUTOFF');
			} break;
		}
	},

	systemS3ButtonController : function(id) {
		var button = document.getElementById(id);
		switch(button.id) {
			case 'systemS3ActionButton' : {
				if(button.value == 'OUTBOARD') {
					this.execCommand('S3', 'DETACH');
				}
			} break;
			case 'systemS3Restart' : {
				this.execCommand('S3', 'RESTART');
			}
		}
	},

	autoPilotButtonController : function(id) {
		var button = document.getElementById(id);
		switch(button.id) {
			case 'autoPilotButton' : {
				if(button.value == 'OFF') {
					this.execCommand('AUTOPILOT', 'STOP');
				} else if(button.value == 'ON') {
					this.execCommand('AUTOPILOT', 'START');
				}
			} break;
		}
	},

	holddownArmsButtonController : function(id) {
		var button = document.getElementById(id);
		switch(button.id) {
			case 'holddownArmsButton' : {
				if(button.value == 'RELEASE') {
					this.execCommand('HOLDDOWN_ARMS', 'STOP');
				}
			} break;
		}
	},

	countdownButtonButtonController : function(id) {
		var button = document.getElementById(id);
		switch(button.id) {
			case 'countdownButton' : {
				if(button.value == 'START') {
					this.execCommand('COUNTDOWN', 'START');
				} else if(button.value == 'STOP') {
					this.execCommand('COUNTDOWN', 'STOP');
				}
			} break;
		}
	},

	chatInputController : function(event) {
		var key = event.keyCode || event.which,
			chatInput = document.getElementById('chatInput');

		if (key == 13 && chatInput.value.length > 0) {
			SATURN_V.controller.Network.sendCommand(chatInput.value, 'chat_message');
			chatInput.value = '';
		}
	}

});