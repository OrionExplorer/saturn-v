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

function parseRemoteData(json) {
	
	if(json.computer_message != lastComputerMessage && json.computer_message.length > 0) {
		var message = '<'+secondsToHms(json.mission_time)+'> '+json.computer_message;
		lastComputerMessage = json.computer_message;
		updateInformation(message);
	}
	updateEl('yawButton', (json.yaw_program_engaged == 1 ? 'OFF' : 'ON' ));
	updateEl('rollButton', (json.roll_program_engaged == 1 ? 'OFF' : 'ON' ));
	updateEl('pitchButton', (json.pitch_program_engaged == 1 ? 'OFF' : 'ON' ));
	//updateEl('voyager7_pitch', Math.round(json.pitch)+' / '+json.dest_pitch/*+'°'*/);
	updateEl('voyager7_pitch', Math.round(json.pitch)/*+'°'*/);
	//updateEl('voyager7_rollAzimuth', Math.round(90-json.roll)+' / '+json.dest_roll/*+'°'*/);
	updateEl('voyager7_rollAzimuth', Math.round(json.roll)/*+'°'*/);
	updateEl('voyager7_yaw', Math.round(json.yaw*10)/10/*+'°'*/);
	//updateEl('voyager7_mainEngine', (json.main_engine_engaged == 1 ? 'ON' : 'OFF' ));

	updateEl('voyager7_s1_fuel', Math.round(json.s_ic_fuel)/*+' KG'*/);
	updateEl('voyager7_s1_burnTime', Math.round(json.s_ic_burn_time)/*+' S'*/);
	updateEl('voyager7_s1_status', (json.s_ic_attached == 1 ? 'OPERATIONAL' : 'STAGED'));
	updateEl('voyager7_s1_thrust', Math.round(json.s_ic_thrust)/*+' N'*/);

	updateEl('voyager7_s2_fuel', Math.round(json.s_ii_fuel)/*+' KG'*/);
	updateEl('voyager7_s2_burnTime', Math.round(json.s_ii_burn_time)/*+' S'*/);
	updateEl('voyager7_s2_status', (json.s_ii_attached == 1 ? 'OPERATIONAL' : 'STAGED'));
	updateEl('voyager7_s2_thrust', Math.round(json.s_ii_thrust)/*+' N'*/);

	updateEl('voyager7_s3_fuel', Math.round(json.s_ivb_fuel)/*+' KG'*/);
	updateEl('voyager7_s3_burnTime', Math.round(json.s_ivb_burn_time)/*+' S'*/);
	updateEl('voyager7_s3_status', (json.s_ivb_attached == 1 ? 'OPERATIONAL' : 'STAGED'));
	updateEl('voyager7_s3_thrust', Math.round(json.s_ivb_thrust)/*+' N'*/);
	
	updateEl('voyager7_mission_time', secondsToHms(json.mission_time));
	updateEl('voyager7_timeInfo', json.current_time_gmt.toUpperCase());
	updateEl('voyager7_velocity', Math.round(json.last_velocity)/* + ' M/S'*/);
	//updateEl('voyager7_verticalVelocity', Math.round(json.current_vertical_velocity) + ' M/S');
	//updateEl('voyager7_horizontalVelocity', Math.round(json.current_horizontal_velocity) + ' M/S');
	//updateEl('voyager7_totalMass', Math.round(json.total_mass) + ' KG');
	updateEl('voyager7_acceleration', Math.round(json.current_acceleration*10)/10 /*+ ' M/S'*/);
	updateEl('voyager7_gForce', json.current_gforce /*+ ' G'*/);
	updateEl('voyager7_dynamicPressure', Math.round(json.current_dynamic_pressure)/10/*+' KG/M²'*/);

	if(json.current_altitude <= 1000) {
		updateEl('voyager7_altitude', Math.round(json.current_altitude) /*+ ' M'*/);
	} else if(json.current_altitude > 1000) {
		updateEl('voyager7_altitude', Math.round(json.current_altitude/100)/10 /*+ ' KM'*/);
	}

	if(json.total_distance <= 1000) {
		updateEl('voyager7_distance', Math.round(json.total_distance) /*+ ' M'*/);
	} else {
		updateEl('voyager7_distance', Math.round(json.total_distance)/1000 /*+ ' KM'*/);
	}
	updateEl('voyager7_thrust', json.current_thrust/*+' %'*/);

	if(json.internal_guidance_engaged == 0) {
		updateEl('mainEngineEngageButton', 'ENGAGE');//, 2000, 'enableEl("'+id+'")');
	} else {
		updateEl('mainEngineEngageButton', 'DISENGAGE');//, 2000, 'enableEl("'+id+'")');
	}

	if(json.main_engine_engaged == 0) {
		updateEl('systemAPSButton', 'ENGAGE');
	} else {
		updateEl('systemAPSButton', 'DISENGAGE');
	}

	if(json.s_ic_center_engine_available == 0) {
		disableEl('systemS1CenterEngineCutoffButton');
	} else {
		enableEl('systemS1CenterEngineCutoffButton');
	}

	if(json.s_ic_attached == 0) {
		disableEl('systemS1ActionButton');
	} else {
		enableEl('systemS1ActionButton');
	}

	if(json.launch_escape_tower_ready == 0) {
		disableEl('towerJettisonButton');
		disableEl('towerJettisonEngageButton');
	} else {
		enableEl('towerJettisonButton');
		enableEl('towerJettisonEngageButton');
	}
	
	updateEl('autoPilotButton', (json.auto_pilot_enabled ? 'OFF' : 'ON'));
	if(json.holddown_arms_released) {
		disableEl('holddownArmsButton');
	} else {
		enableEl('holddownArmsButton');
	}

	updateEl('countdownButton', (json.countdown_in_progress ? 'STOP' : 'START'));

	if(json.countdown_in_progress && json.mission_time > -8 ) {
		disableEl('countdownButton');
	}

	
	if(json.s_ii_center_engine_available == 0) {
		disableEl('systemS2CenterEngineCutoffButton');
	} else {
		enableEl('systemS2CenterEngineCutoffButton');
	}
	
	if(json.s_ii_attached == 0) {
		disableEl('systemS2ActionButton');
	} else {
		enableEl('systemS2ActionButton');
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