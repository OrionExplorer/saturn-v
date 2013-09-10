JSMVC.define('SATURN_V.controller.MainView', {
	name : 'MainView',
	lastComputerMessage : '',
	cachedData : null,
	controlPanelButtons : [
			'controlPanelSIVB_show',
			'controlPanelSII_show',
			'controlPanelSIC_show',
			'controlPanelThrust_show',
			'controlPanelMainEngine_show',
			'controlPanelPitch_show',
			'controlPanelRoll_show',
			'controlPanelYaw_show',
			'controlPanelLET_show',
			'controlPanelAutoPilot_show',
			'controlPanelCountdown_show',
			'controlPanelInternalGuidance_show',
			'controlPanelHolddownArms_show',
			'controlPanelSA514Chat_show',
			'controlPanelMCChat_show'
	],

	getNewData : function(json) {
		var field = null,
			result = {};

		for(field in json) {
			if(json[field] != this.cachedData[field]) {
				result[field] = json[field];
				this.cachedData[field] = json[field];
			}
		}

		return result;
	},

	parseRemoteData : function(json) {
		var newData = null;

		if(this.cachedData == null) {
			this.cachedData = json;
			this.parseLocalData(json);
		} else {
			newData = this.getNewData(json);
			if(JSMVC.Object.isEmpty(newData) === false) {
				this.parseLocalData(newData);
			}
		}
	},

	parseLocalData : function(json) {
		var message = '',
			mission_time = json.mission_time || this.cachedData.mission_time;

		if(json.computer_message && json.computer_message != this.lastComputerMessage && json.computer_message.length > 0) {
			message = '<'+SATURN_V.utils.Shared.secondsToHms(mission_time)+'> '+json.computer_message;
			this.lastComputerMessage = json.computer_message;
			SATURN_V.utils.Frontend.updateInformation(message);
		}

		if(json.yaw_program_engaged != undefined) {
			SATURN_V.utils.Frontend.updateEl('yawButton', (json.yaw_program_engaged == 1 ? 'OFF' : 'ON' ));	
		}
		
		if(json.roll_program_engaged != undefined) {
			SATURN_V.utils.Frontend.updateEl('rollButton', (json.roll_program_engaged == 1 ? 'OFF' : 'ON' ));	
		}
		
		if(json.pitch_program_engaged != undefined) {
			SATURN_V.utils.Frontend.updateEl('pitchButton', (json.pitch_program_engaged == 1 ? 'OFF' : 'ON' ));
		}

		if(json.pitch != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_pitch', Math.round(json.pitch)/*+'°'*/);
			this.updateRocketPitch(json.pitch);
		}
		
		if(json.roll != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_rollAzimuth', Math.round(json.roll)/*+'°'*/);	
		}
		
		if(json.yaw != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_yaw', Math.round(json.yaw*10)/10/*+'°'*/);	
		}
		
		if(json.orbit_apoapsis != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_orbitA', Math.round(json.orbit_apoapsis/100)/10);	
		}
		
		if(json.orbit_periapsis != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_orbitP', Math.round(json.orbit_periapsis/100)/10);	
		}
		
		if(json.orbit_inclination != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_orbitInclination', Math.round(json.orbit_inclination));	
		}

		if(json.s_ic_fuel != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_s1_fuel', Math.round(json.s_ic_fuel)/*+' KG'*/);	
		}
		
		if(json.s_ic_burn_time != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_s1_burnTime', Math.round(json.s_ic_burn_time)/*+' S'*/);	
		}
		
		if(json.s_ic_attached != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_s1_status', (json.s_ic_attached == 1 ? 'OPERATIONAL' : 'STAGED'));
		}
		if(json.s_ic_thrust != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_s1_thrust', Math.round(json.s_ic_thrust)/*+' N'*/);	
		}

		if(json.s_ii_fuel != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_s2_fuel', Math.round(json.s_ii_fuel)/*+' KG'*/);	
		}
		
		if(json.s_ii_burn_time != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_s2_burnTime', Math.round(json.s_ii_burn_time)/*+' S'*/);	
		}
		
		if(json.s_ii_attached != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_s2_status', (json.s_ii_attached == 1 ? 'OPERATIONAL' : 'STAGED'));	
		}
		
		if(json.s_ii_thrust != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_s2_thrust', Math.round(json.s_ii_thrust)/*+' N'*/);	
		}

		if(json.s_ivb_fuel != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_s3_fuel', Math.round(json.s_ivb_fuel)/*+' KG'*/);	
		}
		
		if(json.s_ivb_burn_time != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_s3_burnTime', Math.round(json.s_ivb_burn_time)/*+' S'*/);	
		}
		
		if(json.s_ivb_attached != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_s3_status', (json.s_ivb_attached == 1 ? 'OPERATIONAL' : 'STAGED'));	
		}
		
		if(json.s_ivb_thrust != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_s3_thrust', Math.round(json.s_ivb_thrust)/*+' N'*/);	
		}
		
		SATURN_V.utils.Frontend.updateEl('voyager7_mission_time', SATURN_V.utils.Shared.secondsToHms(mission_time));
		document.title = 'SATURN V ('+SATURN_V.utils.Shared.secondsToHms(mission_time)+')';
		
		if(json.current_time_gmt != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_timeInfo', json.current_time_gmt.toUpperCase());	
		}

		if(json.last_velocity != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_velocity', Math.round(json.last_velocity)/* + ' M/S'*/);	
		}
		
		if(json.current_acceleration != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_acceleration', Math.round(json.current_acceleration*10)/10 /*+ ' M/S'*/);	
		}
		
		if(json.current_gforce != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_gForce', json.current_gforce /*+ ' G'*/);	
		}
		
		if(json.current_dynamic_pressure != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_dynamicPressure', Math.round(json.current_dynamic_pressure)/10/*+' KG/M²'*/);	
		}

		if(json.current_altitude != undefined) {
			if(json.current_altitude <= 1000) {
				SATURN_V.utils.Frontend.updateEl('voyager7_altitude', Math.round(json.current_altitude) /*+ ' M'*/);
			} else if(json.current_altitude > 1000) {
					SATURN_V.utils.Frontend.updateEl('voyager7_altitude', Math.round(json.current_altitude/100)/10 /*+ ' KM'*/);		
			}
		}

		if(json.total_distance  != undefined) {
			if(json.total_distance <= 1000) {
				SATURN_V.utils.Frontend.updateEl('voyager7_distance', Math.round(json.total_distance) /*+ ' M'*/);
			} else {
				SATURN_V.utils.Frontend.updateEl('voyager7_distance', Math.round(json.total_distance)/1000 /*+ ' KM'*/);
			}	
		}
		

		if(json.current_thrust != undefined) {
			SATURN_V.utils.Frontend.updateEl('voyager7_thrust', json.current_thrust/*+' %'*/);	
		}

		if(json.internal_guidance_engaged != undefined) {
			if(json.internal_guidance_engaged == 0) {
				SATURN_V.utils.Frontend.updateEl('internalGuidanceButton', 'ENGAGE');//, 2000, 'SATURN_V.utils.Frontend.enableEl("'+id+'")');
			} else {
				SATURN_V.utils.Frontend.updateEl('internalGuidanceButton', 'DISENGAGE');//, 2000, 'SATURN_V.utils.Frontend.enableEl("'+id+'")');
			}
		}

		if(json.main_engine_engaged != undefined) {
			if(json.main_engine_engaged == 0) {
				SATURN_V.utils.Frontend.updateEl('mainEngineEngageButton', 'ENGAGE');
			} else {
				SATURN_V.utils.Frontend.updateEl('mainEngineEngageButton', 'DISENGAGE');
			}	
		}

		if(json.s_ic_center_engine_available != undefined) {
			if(json.s_ic_center_engine_available == 0) {
				SATURN_V.utils.Frontend.disableEl('systemS1CenterEngineCutoffButton');
			} else {
				SATURN_V.utils.Frontend.enableEl('systemS1CenterEngineCutoffButton');
			}	
		}

		if(json.s_ic_attached != undefined) {
			if(json.s_ic_attached == 0) {
				SATURN_V.utils.Frontend.disableEl('systemS1ActionButton');
			} else {
				SATURN_V.utils.Frontend.enableEl('systemS1ActionButton');
			}	
		}

		if(json.launch_escape_tower_ready != undefined) {
			if(json.launch_escape_tower_ready == 0) {
				SATURN_V.utils.Frontend.disableEl('towerJettisonButton');
				SATURN_V.utils.Frontend.disableEl('towerJettisonEngageButton');
			} else {
				SATURN_V.utils.Frontend.enableEl('towerJettisonButton');
				SATURN_V.utils.Frontend.enableEl('towerJettisonEngageButton');
			}	
		}
		
		if(json.auto_pilot_enabled != undefined) {
			SATURN_V.utils.Frontend.updateEl('autoPilotButton', (json.auto_pilot_enabled ? 'OFF' : 'ON'));	
		}
		
		if(json.holddown_arms_released != undefined) {
			SATURN_V.utils.Frontend.disableEl('holddownArmsButton');
		} else {
			SATURN_V.utils.Frontend.enableEl('holddownArmsButton');
		}

		if(json.countdown_in_progress != undefined) {
			SATURN_V.utils.Frontend.updateEl('countdownButton', (json.countdown_in_progress ? 'STOP' : 'START'));	
		}

		if(json.countdown_in_progress != undefined) {
			if(mission_time > -8 ) {
				SATURN_V.utils.Frontend.disableEl('countdownButton');
			}	
		}

		if(json.s_ii_center_engine_available != undefined) {
			if(json.s_ii_center_engine_available == 0) {
				SATURN_V.utils.Frontend.disableEl('systemS2CenterEngineCutoffButton');
			} else {
				SATURN_V.utils.Frontend.enableEl('systemS2CenterEngineCutoffButton');
			}
		}

		if(json.s_ii_interstage_mass != undefined) {
			if(json.s_ii_interstage_mass > 0) {
				SATURN_V.utils.Frontend.enableEl('systemS2InterstageJettisonButton');
			} else {
				SATURN_V.utils.Frontend.disableEl('systemS2InterstageJettisonButton');
			}	
		}
		
		if(json.s_ii_attached != undefined) {
			if(json.s_ii_attached == 0) {
				SATURN_V.utils.Frontend.disableEl('systemS2ActionButton');
			} else {
				SATURN_V.utils.Frontend.enableEl('systemS2ActionButton');
			}	
		}

		if(json.launch_escape_tower_ready != undefined
			|| json.active_stage != undefined
			|| json.s_ii_interstage_mass != undefined
		) {
			this.updateRocketView(this.cachedData.launch_escape_tower_ready, this.cachedData.active_stage);
		}
	},

	updateRocketPitch : function(pitch_value) {
		var rocketView = SATURN_V.utils.Frontend.findElementsByDataAttr('component', 'rocket-display'),
		i = 0;

		for(i = 0; i < rocketView.length; i++) {
			rocketView[i].style.transform = 'rotate('+pitch_value+'deg)';
			rocketView[i].style['-webkit-transform'] = 'rotate('+pitch_value+'deg)';
		}
	},

	updateRocketView : function(launch_escape_tower_ready, active_stage) {
		var rocketView = SATURN_V.utils.Frontend.findElementsByDataAttr('component', 'rocket-display'),
		i = 0,
		styleStr = '';

		if(active_stage === 1) {
			styleStr = 'url(img/01'+(launch_escape_tower_ready ? '' : '_NOLET')+'.png)';
		} else if(active_stage === 2) {
			if(this.cachedData.s_ii_interstage_mass > 0) {
				styleStr = 'url(img/02'+(launch_escape_tower_ready ? '' : '_NOLET')+'.png)';
			} else {
				styleStr = 'url(img/03'+(launch_escape_tower_ready ? '' : '_NOLET')+'.png)';
			}
		} else if(active_stage === 3) {
			styleStr = 'url(img/04.png)';
		}

		for(i = 0; i < rocketView.length; i++) {
			rocketView[i].style.backgroundImage = styleStr;
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
	},

	closeControlPanel : function() {
		document.getElementById('controlPanel').style.display = 'none';
	},

	setControlPanelButtonsVisible : function(bool) {
		var i = 0,
			element = null,
			targetId = null;

		for(i = 0; i < this.controlPanelButtons.length; i++) {
			targetId = this.controlPanelButtons[i].split('_show')[0];
			element = document.getElementById(targetId);
			element.style.display = (bool === true ? 'block' : 'none');
		}
	},

	showControlPanel : function(reffererId) {
		var i = 0,
			element = null,
			targetId = reffererId[0] ? reffererId[0].split('_show')[0] : null,
			controlPanel = document.getElementById('controlPanel'),
			controlZone = SATURN_V.utils.Frontend.findElementsByDataAttr('name','controlZone')[0] || null;

		this.setControlPanelButtonsVisible(false);
		if(targetId) {
			document.getElementById(targetId).style.display = 'block';
			controlPanel.style.display = 'block';
		}
		
	}
});