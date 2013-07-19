JSMVC.define('SATURN_V.controller.ControlPanel.ControlPanel', {
	views : [],
	models : [],
	name : 'ControlPanel',

	lastComputerMessage : '',

	init : function() {
		
	},

	parseRemoteData : function(json) {
		if(json.computer_message != this.lastComputerMessage && json.computer_message.length > 0) {
			var message = '<'+SATURN_V.utils.Shared.secondsToHms(json.mission_time)+'> '+json.computer_message;
			this.lastComputerMessage = json.computer_message;
			SATURN_V.utils.Frontend.updateInformation(message);
		}
		SATURN_V.utils.Frontend.updateEl('yawButton', (json.yaw_program_engaged == 1 ? 'OFF' : 'ON' ));
		SATURN_V.utils.Frontend.updateEl('rollButton', (json.roll_program_engaged == 1 ? 'OFF' : 'ON' ));
		SATURN_V.utils.Frontend.updateEl('pitchButton', (json.pitch_program_engaged == 1 ? 'OFF' : 'ON' ));
		SATURN_V.utils.Frontend.updateEl('voyager7_pitch', Math.round(json.pitch)/*+'°'*/);
		SATURN_V.utils.Frontend.updateEl('voyager7_rollAzimuth', Math.round(json.roll)/*+'°'*/);
		SATURN_V.utils.Frontend.updateEl('voyager7_yaw', Math.round(json.yaw*10)/10/*+'°'*/);

		SATURN_V.utils.Frontend.updateEl('voyager7_orbitA', Math.round(json.orbit_apoapsis/100)/10);
		SATURN_V.utils.Frontend.updateEl('voyager7_orbitP', Math.round(json.orbit_periapsis/100)/10);
		SATURN_V.utils.Frontend.updateEl('voyager7_orbitInclination', Math.round(json.orbit_inclination));

		SATURN_V.utils.Frontend.updateEl('voyager7_s1_fuel', Math.round(json.s_ic_fuel)/*+' KG'*/);
		SATURN_V.utils.Frontend.updateEl('voyager7_s1_burnTime', Math.round(json.s_ic_burn_time)/*+' S'*/);
		SATURN_V.utils.Frontend.updateEl('voyager7_s1_status', (json.s_ic_attached == 1 ? 'OPERATIONAL' : 'STAGED'));
		SATURN_V.utils.Frontend.updateEl('voyager7_s1_thrust', Math.round(json.s_ic_thrust)/*+' N'*/);

		SATURN_V.utils.Frontend.updateEl('voyager7_s2_fuel', Math.round(json.s_ii_fuel)/*+' KG'*/);
		SATURN_V.utils.Frontend.updateEl('voyager7_s2_burnTime', Math.round(json.s_ii_burn_time)/*+' S'*/);
		SATURN_V.utils.Frontend.updateEl('voyager7_s2_status', (json.s_ii_attached == 1 ? 'OPERATIONAL' : 'STAGED'));
		SATURN_V.utils.Frontend.updateEl('voyager7_s2_thrust', Math.round(json.s_ii_thrust)/*+' N'*/);

		SATURN_V.utils.Frontend.updateEl('voyager7_s3_fuel', Math.round(json.s_ivb_fuel)/*+' KG'*/);
		SATURN_V.utils.Frontend.updateEl('voyager7_s3_burnTime', Math.round(json.s_ivb_burn_time)/*+' S'*/);
		SATURN_V.utils.Frontend.updateEl('voyager7_s3_status', (json.s_ivb_attached == 1 ? 'OPERATIONAL' : 'STAGED'));
		SATURN_V.utils.Frontend.updateEl('voyager7_s3_thrust', Math.round(json.s_ivb_thrust)/*+' N'*/);
		
		SATURN_V.utils.Frontend.updateEl('voyager7_mission_time', SATURN_V.utils.Shared.secondsToHms(json.mission_time));
		document.title = 'SATURN V ('+SATURN_V.utils.Shared.secondsToHms(json.mission_time)+')';
		SATURN_V.utils.Frontend.updateEl('voyager7_timeInfo', json.current_time_gmt.toUpperCase());
		SATURN_V.utils.Frontend.updateEl('voyager7_velocity', Math.round(json.last_velocity)/* + ' M/S'*/);
		SATURN_V.utils.Frontend.updateEl('voyager7_acceleration', Math.round(json.current_acceleration*10)/10 /*+ ' M/S'*/);
		SATURN_V.utils.Frontend.updateEl('voyager7_gForce', json.current_gforce /*+ ' G'*/);
		SATURN_V.utils.Frontend.updateEl('voyager7_dynamicPressure', Math.round(json.current_dynamic_pressure)/10/*+' KG/M²'*/);

		if(json.current_altitude <= 1000) {
			SATURN_V.utils.Frontend.updateEl('voyager7_altitude', Math.round(json.current_altitude) /*+ ' M'*/);
		} else if(json.current_altitude > 1000) {
			SATURN_V.utils.Frontend.updateEl('voyager7_altitude', Math.round(json.current_altitude/100)/10 /*+ ' KM'*/);
		}

		if(json.total_distance <= 1000) {
			SATURN_V.utils.Frontend.updateEl('voyager7_distance', Math.round(json.total_distance) /*+ ' M'*/);
		} else {
			SATURN_V.utils.Frontend.updateEl('voyager7_distance', Math.round(json.total_distance)/1000 /*+ ' KM'*/);
		}
		SATURN_V.utils.Frontend.updateEl('voyager7_thrust', json.current_thrust/*+' %'*/);

		if(json.internal_guidance_engaged == 0) {
			SATURN_V.utils.Frontend.updateEl('mainEngineEngageButton', 'ENGAGE');//, 2000, 'SATURN_V.utils.Frontend.enableEl("'+id+'")');
		} else {
			SATURN_V.utils.Frontend.updateEl('mainEngineEngageButton', 'DISENGAGE');//, 2000, 'SATURN_V.utils.Frontend.enableEl("'+id+'")');
		}

		if(json.main_engine_engaged == 0) {
			SATURN_V.utils.Frontend.updateEl('systemAPSButton', 'ENGAGE');
		} else {
			SATURN_V.utils.Frontend.updateEl('systemAPSButton', 'DISENGAGE');
		}

		if(json.s_ic_center_engine_available == 0) {
			SATURN_V.utils.Frontend.disableEl('systemS1CenterEngineCutoffButton');
		} else {
			SATURN_V.utils.Frontend.enableEl('systemS1CenterEngineCutoffButton');
		}

		if(json.s_ic_attached == 0) {
			SATURN_V.utils.Frontend.disableEl('systemS1ActionButton');
		} else {
			SATURN_V.utils.Frontend.enableEl('systemS1ActionButton');
		}

		if(json.launch_escape_tower_ready == 0) {
			SATURN_V.utils.Frontend.disableEl('towerJettisonButton');
			SATURN_V.utils.Frontend.disableEl('towerJettisonEngageButton');
		} else {
			SATURN_V.utils.Frontend.enableEl('towerJettisonButton');
			SATURN_V.utils.Frontend.enableEl('towerJettisonEngageButton');
		}
		
		SATURN_V.utils.Frontend.updateEl('autoPilotButton', (json.auto_pilot_enabled ? 'OFF' : 'ON'));
		if(json.holddown_arms_released) {
			SATURN_V.utils.Frontend.disableEl('holddownArmsButton');
		} else {
			SATURN_V.utils.Frontend.enableEl('holddownArmsButton');
		}

		SATURN_V.utils.Frontend.updateEl('countdownButton', (json.countdown_in_progress ? 'STOP' : 'START'));

		if(json.countdown_in_progress && json.mission_time > -8 ) {
			SATURN_V.utils.Frontend.disableEl('countdownButton');
		}

		
		if(json.s_ii_center_engine_available == 0) {
			SATURN_V.utils.Frontend.disableEl('systemS2CenterEngineCutoffButton');
		} else {
			SATURN_V.utils.Frontend.enableEl('systemS2CenterEngineCutoffButton');
		}
		
		if(json.s_ii_attached == 0) {
			SATURN_V.utils.Frontend.disableEl('systemS2ActionButton');
		} else {
			SATURN_V.utils.Frontend.enableEl('systemS2ActionButton');
		}

	},

	showApolloLMControlPanel : function () {
		var apolloCSM_control = document.getElementById('apolloCSM_control'),
			apolloLM_control = document.getElementById('apolloLM_control'),
			sa514_control = document.getElementById('sa514_control'),
			houstonMC_control = document.getElementById('houstonMC_control');

		houstonMC_control.style.display = 'none';
		apolloCSM_control.style.display = 'none';
		sa514_control.style.display = 'none';
		apolloLM_control.style.display = 'block';
	},

	showApolloCSMControlPanel : function () {
		var apolloCSM_control = document.getElementById('apolloCSM_control'),
			apolloLM_control = document.getElementById('apolloLM_control'),
			sa514_control = document.getElementById('sa514_control'),
			houstonMC_control = document.getElementById('houstonMC_control');

		houstonMC_control.style.display = 'none';
		apolloLM_control.style.display = 'none';
		sa514_control.style.display = 'none';
		apolloCSM_control.style.display = 'block';
	},

	showApolloSA514ControlPanel : function () {
		var apolloCSM_control = document.getElementById('apolloCSM_control'),
			apolloLM_control = document.getElementById('apolloLM_control'),
			sa514_control = document.getElementById('sa514_control'),
			houstonMC_control = document.getElementById('houstonMC_control');

		houstonMC_control.style.display = 'none';
		apolloLM_control.style.display = 'none';
		apolloCSM_control.style.display = 'none';
		sa514_control.style.display = 'block';
	},

	showHoustonMCControlPanel : function () {
		var apolloCSM_control = document.getElementById('apolloCSM_control'),
			apolloLM_control = document.getElementById('apolloLM_control'),
			sa514_control = document.getElementById('sa514_control'),
			houstonMC_control = document.getElementById('houstonMC_control');

		apolloLM_control.style.display = 'none';
		apolloCSM_control.style.display = 'none';
		sa514_control.style.display = 'none';
		houstonMC_control.style.display = 'block';
	}
});