var lastComputerMessage = '';

function kotlecikButtonController(id) {
	var button = document.getElementById(id);
	Socket.send('konsumpcja kotlecika!');
	
}

function pitchRollYawButtonController(id) {
	var button = document.getElementById(id);
	var buttonVal = (button.value == 'ON' ? 'START' : 'STOP');
	switch(button.id) {
		/* PITCH CONTROL */
		case 'pitchButton' : {
				execCommand('PITCH_PROGRAM', buttonVal);
		} break;
		case 'pitchIncButton' : {
				execCommand('PITCH_MOD', 'INCREASE', 1);
		} break;
		case 'pitchDecButton' : {
				execCommand('PITCH_MOD', 'DECREASE', -1);
		} break;
		/* ROLL CONTROL */
		case 'rollButton' : {
				execCommand('ROLL_PROGRAM', buttonVal);
		} break;
		case 'rollIncButton' : {
				execCommand('ROLL_MOD', 'INCREASE', 1);
		} break;
		case 'rollDecButton' : {
				execCommand('ROLL_MOD', 'DECREASE', -1);
		} break;
		/* YAW CONTROL */
		case 'yawButton' : {
				execCommand('YAW_PROGRAM', buttonVal);
		} break;
		case 'yawIncButton' : {
				execCommand('YAW_MOD', 'INCREASE', 1);
		} break;
		case 'yawDecButton' : {
				execCommand('YAW_MOD', 'DECREASE', -1);
		} break;
	}
}

function mainEngineEngageButtonController(id) {
	switch(document.getElementById(id).value) {
		case 'ENGAGE' : {
				execCommand('INTERNAL_GUIDANCE', 'START');
				
		}break;
		case 'DISENGAGE' : {
				execCommand('INTERNAL_GUIDANCE', 'STOP');
		}break;
	}
}

function systemMPSButtonController(id) {
	var button = document.getElementById(id);
	switch(button.id) {
		case 'systemAPSButton' : {
				if(button.value == 'ENGAGE') {
					execCommand('MAIN_ENGINE', 'START');
				} else {
					execCommand('MAIN_ENGINE', 'STOP');
				}
		} break;
	}
}

function mainEngineThrustButtonController(id) {
	switch(document.getElementById(id).value) {
		case '+' : {
				execCommand('THRUST', 'INCREASE', 5, false);
		} break;
		case '-' : {
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
			execCommand('LET', 'JETTISON');
		} break;
	}
}

function systemS1ButtonController(id) {
	var button = document.getElementById(id);
	switch(button.id) {
		case 'systemS1ActionButton' : {
			if(button.value == 'OUTBOARD') {
				execCommand('S1', 'DETACH');
			}
		} break;
		case 'systemS1CenterEngineCutoffButton' : {
			execCommand('S1', 'CENTER_ENGINE_CUTOFF', 20, true)
		} break;
	}
}

function systemS2ButtonController(id) {
	var button = document.getElementById(id);
	switch(button.id) {
		case 'systemS2ActionButton' : {
			if(button.value == 'OUTBOARD') {
				execCommand('S2', 'DETACH');
			}
		} break;
		case 'systemS2CenterEngineCutoffButton' : {
			execCommand('S2', 'CENTER_ENGINE_CUTOFF');
		} break;
	}
}

function systemS3ButtonController(id) {
	var button = document.getElementById(id);
	switch(button.id) {
		case 'systemS3ActionButton' : {
			if(button.value == 'OUTBOARD') {
				execCommand('S3', 'DETACH');
			}
		} break;
		case 'systemS3Restart' : {
			execCommand('S3', 'RESTART');
		}
	}
}

function autoPilotButtonController(id) {
	var button = document.getElementById(id);
	switch(button.id) {
		case 'autoPilotButton' : {
			if(button.value == 'OFF') {
				execCommand('AUTOPILOT', 'STOP');
			} else if(button.value == 'ON') {
				execCommand('AUTOPILOT', 'START');
			}
		} break;
	}
}

function holddownArmsButtonController(id) {
	var button = document.getElementById(id);
	switch(button.id) {
		case 'holddownArmsButton' : {
			if(button.value == 'RELEASE') {
				execCommand('HOLDDOWN_ARMS', 'STOP');
			}
		} break;
	}
}

function countdownButtonButtonController(id) {
	var button = document.getElementById(id);
	switch(button.id) {
		case 'countdownButton' : {
			if(button.value == 'START') {
				execCommand('COUNTDOWN', 'START');
			} else if(button.value == 'STOP') {
				execCommand('COUNTDOWN', 'STOP');
			}
		} break;
	}
}

function chatInputController(event) {
	var key = event.keyCode || event.which;
	var chatInput = document.getElementById('chat-input');

	if (key == 13 && chatInput.value.length > 0) {
		sendCommand(chatInput.value, 'chat_message');
		chatInput.value = '';
	}
}

function execCommand(device, command, value) {
	var commandStr = '';
	
	var devices = {
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
	};
	
	var commands = {
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
	
	if(Socket) {
		commandStr = devices[device]+' '+commands[command]+' '+(value ? value : '0');
		sendCommand(commandStr, 'computer');
	}
}

function secondsToHms(d) {
	var lessThanZero = (d < 0) ? true : false;
	d = Math.abs(Math.round(Number(d)));

	var h = Math.floor(d / 3600);
	var m = Math.floor(d % 3600 / 60);
	var s = Math.floor(d % 3600 % 60);
	return lessThanZero ? 'T-'+(h < 10 ? '0'+h : h)+':'+(m < 10 ? '0'+m : m)+':'+(s < 10 ? '0'+s : s) : 'T+'+(h < 10 ? '0'+h : h)+':'+(m < 10 ? '0'+m : m)+':'+(s < 10 ? '0'+s : s);
}

function hmsToSeconds(hms) {
	var a = hms.split(':');
	var seconds = (+a[0]) * 60 * 60 + (+a[1]) * 60 + (+a[2]); 
	return seconds;
}