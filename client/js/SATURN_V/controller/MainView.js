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
			this.updateRocketYaw(json.yaw);
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

		if(json.launch_escape_tower_ready != undefined && json.launch_escape_tower_ready == false) {
			this.updateRocketView('let_jettison');
		}
		if(json.active_stage != undefined) {
			this.updateRocketView('staging');
		}
		if(json.s_ii_interstage_mass != undefined && json.s_ii_interstage_mass == 0) {
			this.updateRocketView('s_ii_interstage_jettison');
		}
	},

	updateRocketView : function(action) {
		var rocketView = SATURN_V.utils.Frontend.findElementsByDataAttr('component', 'rocket-display'),
		i = 0, j = 0,
		styleStr = '',
		animationStr = '';
		currentStage = this.cachedData.active_stage,
		currentLETavailable = this.cachedData.launch_escape_tower_ready,
		currentSIIinterstageAvailable = (this.cachedData.s_ii_interstage_mass > 0);

		switch(action) {
			case 'let_jettison' : {
				if(currentStage == 1) { /* Let jettison while S-IC is available */
					styleStr = 'url(img/01_NOLET.png)';
					animationStr = 'LET_JETTISON_SIC';
				} else if(currentStage == 2) { /* Let jettison while S-II is available */
					if(currentSIIinterstageAvailable == true) { /* S-II interstage is available */
						styleStr = 'url(img/02_NOLET.png)';
						animationStr = 'LET_JETTISON_SII_INT';
					} else { /* S-II interstage is unavailable */
						styleStr = 'url(img/03_NOLET.png)';
						animationStr = 'LET_JETTISON_SII';
					}
				} else if(currentStage == 3) { /* Let jettison while S-IVB is available */
					styleStr = 'url(img/04.png)';
				}
			} break;
			case 'staging' : {
				if(currentLETavailable == true) {
					if(currentStage == 2) { /* S-IC separation while LET is available*/
						styleStr = 'url(img/02.png)';
						animationStr = 'STAGING_SIC_LET';
					} else if(currentStage == 3) { /* S-II separation while LET is available (!?) */
						styleStr = 'url(img/04)';
						animationStr = 'STAGING_SII_LET';
					}
				} else {
					if(currentStage == 2) { /* S-IC separation while LET is unavailable*/
						styleStr = 'url(img/02_NOLET.png)';
						animationStr = 'STAGING_SIC';
					} else if(currentStage == 3) { /* S-II separation */
						styleStr = 'url(img/04.png)';
						animationStr = 'STAGING_SII';
					}
				}
			} break;
			case 's_ii_interstage_jettison' : {
				if(currentLETavailable == true) {	/* S-II interstage jettison while LET is available */
					styleStr = 'url(img/03.png)';
					animationStr = 'SII_INTERSTAGE_JETTISON_LET';
				} else { /* S-II interstage jettison while LET is unavailable */
					styleStr = 'url(img/03_NOLET.png)'
					animationStr = 'SII_INTERSTAGE_JETTISON';
				}
			} break;
		}

		for(i = 0; i < rocketView.length; i++) {
			if(currentStage > 1) {
				rocketView[i].style.animation = animationStr+' 4s';
				rocketView[i].style['-webkit-animation'] = animationStr+' 4s';
			}
			rocketView[i].style.backgroundImage = styleStr;
		}

		/* We have to remove all animations becouse of infinite playing while switching between panels */
		setTimeout(function() {
			for(j = 0; j < rocketView.length; j++) {
				rocketView[j].style.animation = null;
				rocketView[j].style['-webkit-animation'] = null;
			}
		}, 3000);
	},

	updateRocketPitch : function(pitch_value) {
		var rocketView = SATURN_V.utils.Frontend.findElementsByDataAttr('component', 'rocket-display'),
		i = 0;

		for(i = 0; i < rocketView.length; i++) {
			rocketView[i].style.transform = 'rotate('+pitch_value+'deg)';
			rocketView[i].style['-webkit-transform'] = 'rotate('+pitch_value+'deg)';
		}
	},

	updateRocketYaw : function(yaw_value) {
		var rocketView = SATURN_V.utils.Frontend.findElementsByDataAttr('component', 'rocket-display'),
		i = 0;

		for(i = 0; i < rocketView.length; i++) {
			rocketView[i].style.transform = 'rotate('+(-yaw_value)+'deg)';
			rocketView[i].style['-webkit-transform'] = 'rotate('+(-yaw_value)+'deg)';
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