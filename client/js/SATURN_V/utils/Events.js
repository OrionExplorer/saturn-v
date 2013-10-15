JSMVC.define('SATURN_V.utils.Events', {
	
	init : function() {
		this.loginButton();
		this.closeControlPanelButton();
		this.onSaturnVLabelClick();
		this.onMissionTimeLabelClick();

		this.showCSMTab();
		this.showSA514Tab();
		this.showLMTab();

		this.countdownButtonButtonController();
		this.holddownArmsButtonController();
		this.internalGuidanceButtonController();
		this.iterativeGuidanceModeButtonController();

		this.mainEngineEngageButtonController();
		this.mainEngineThrustButtonController();
		this.systemS1ButtonController();
		this.systemS2ButtonController();
		this.systemS3ButtonController();
		this.pitchRollYawButtonController();
		this.towerJettisonButtonController();

		this.autoPilotButtonController();
		this.chatInputController();
		this.showControlPanelButtons();
	},

	showControlPanelButtons : function() {
		var controlPanelButtons = SATURN_V.controller.MainView.controlPanelButtons,
			i = 0,
			eventsList = [];

		for(i = 0; i < controlPanelButtons.length; i++) {
			eventsList[eventsList.length] = {
				elementId : controlPanelButtons[i],
				eventName : 'click',
				callback : SATURN_V.controller.MainView.showControlPanel,
				callbackArguments : [controlPanelButtons[i]],
				scope : SATURN_V.controller.MainView
			}
		}
		this.registerEvent(eventsList);
	},

	registerEvent : function(cfg) {
		var i = 0,
			listener = function(index) {
				var element = document.getElementById(cfg[index].elementId),
				single = true,
				i = 0;

				if(!element) {
					element = SATURN_V.utils.Frontend.findElementsByDataAttr('component', cfg[index].elementId);
					if(!element) {
						console.error('Cannot find element "'+cfg[index].elementId+'" in DOM.')
					} else {
						single = false;
					}
				}

				if(element) {
					if(single === true) {
						document.getElementById(cfg[index].elementId).addEventListener(cfg[index].eventName, function(event) {
							cfg[index].callback.call(cfg[index].scope, cfg[index].callbackArguments ? cfg[index].callbackArguments : event);
						});
					} else {
						for(i = 0; i < element.length; i++) {
							element[i].addEventListener(cfg[index].eventName, function(event) {
								cfg[index].callback.call(cfg[index].scope, cfg[index].callbackArguments ? cfg[index].callbackArguments : event);
							});
						}
					}
				}
			};

		for(i = 0; i < cfg.length; i++) {
			listener(i);
		}
	},

	showCSMTab : function() {
		this.registerEvent([{
			elementId : 'show-csm-tab',
			eventName : 'click',
			callback : SATURN_V.controller.MainView.showCSMTab,
			callbackArguments : null,
			scope : SATURN_V.controller.MainView
		}]);
	},

	showSA514Tab : function() {
		this.registerEvent([{
			elementId : 'show-sa514-tab',
			eventName : 'click',
			callback : SATURN_V.controller.MainView.showSA514Tab,
			callbackArguments : null,
			scope : SATURN_V.controller.MainView
		}]);
	},

	showLMTab : function() {
		this.registerEvent([{
			elementId : 'show-lm-tab',
			eventName : 'click',
			callback : SATURN_V.controller.MainView.showLMTab,
			callbackArguments : null,
			scope : SATURN_V.controller.MainView
		}]);
	},

	chatInputController : function() {
		this.registerEvent([{
			elementId : 'chatInput',
			eventName : 'keypress',
			callback : SATURN_V.controller.MainComputer.chatInputController,
			callbackArguments : null,
			scope : SATURN_V.controller.MainComputer
		}]);
	},

	autoPilotButtonController : function() {
		this.registerEvent([{
			elementId : 'autoPilotButton',
			eventName : 'click',
			callback : SATURN_V.controller.MainComputer.autoPilotButtonController,
			callbackArguments : ['autoPilotButton'],
			scope : SATURN_V.controller.MainComputer
		}]);
	},

	towerJettisonButtonController : function() {
		this.registerEvent([{
			elementId : 'towerJettisonButton',
			eventName : 'click',
			callback : SATURN_V.controller.MainComputer.towerJettisonButtonController,
			callbackArguments : ['towerJettisonButton'],
			scope : SATURN_V.controller.MainComputer
		}, {
			elementId : 'towerJettisonEngageButton',
			eventName : 'click',
			callback : SATURN_V.controller.MainComputer.towerJettisonButtonController,
			callbackArguments : ['towerJettisonEngageButton'],
			scope : SATURN_V.controller.MainComputer
		}]);
	},

	loginButton : function() {
		this.registerEvent([{
			elementId : 'loginButton',
			eventName : 'click',
			callback : SATURN_V.controller.Network.performUserLogin,
			scope : SATURN_V.controller.Network
		}]);
	},

	closeControlPanelButton : function() {
		this.registerEvent([{
			elementId : 'closeControlPanelButton',
			eventName : 'click',
			callback : SATURN_V.controller.MainView.closeControlPanel,
			scope : SATURN_V.controller.MainView
		}]);
	},

	mainEngineEngageButtonController : function() {
		this.registerEvent([{
			elementId : 'mainEngineEngageButton',
			eventName : 'click',
			callback : SATURN_V.controller.MainComputer.mainEngineEngageButtonController,
			callbackArguments : ['mainEngineEngageButton'],
			scope : SATURN_V.controller.MainComputer
		}]);
	},

	iterativeGuidanceModeButtonController : function() {
		this.registerEvent([{
			elementId : 'iterativeGuidanceModeEngageButton',
			eventName : 'click',
			callback : SATURN_V.controller.MainComputer.iterativeGuidanceModeButtonController,
			callbackArguments : ['iterativeGuidanceModeEngageButton'],
			scope : SATURN_V.controller.MainComputer
		}]);	
	},

	mainEngineThrustButtonController : function() {
		var buttons = [
			'mainEngineThrustFullButton',
			'mainEngineThrustUpButton',
			'mainEngineThrustDownButton',
			'mainEngineThrustNullButton'
		],
			i = 0,
			items = [];

		for(i = 0; i < buttons.length; i++) {
			items[items.length] = {
				elementId : buttons[i],
				eventName : 'click',
				callback : SATURN_V.controller.MainComputer.mainEngineThrustButtonController,
				callbackArguments : [buttons[i]],
				scope : SATURN_V.controller.MainComputer
			}
		}

		this.registerEvent(items);
	},

	pitchRollYawButtonController : function() {
		var buttons = [
			'pitchButton',
			'rollButton',
			'yawButton',
			'pitchIncButton',
			'pitchDecButton',
			'rollIncButton',
			'rollDecButton',
			'yawIncButton',
			'yawDecButton'
		],
			i = 0,
			items = [];

		for(i = 0; i < buttons.length; i++) {
			items[items.length] = {
				elementId : buttons[i],
				eventName : 'click',
				callback : SATURN_V.controller.MainComputer.pitchRollYawButtonController,
				callbackArguments : [buttons[i]],
				scope : SATURN_V.controller.MainComputer
			}
		}

		this.registerEvent(items);
	},

	systemS1ButtonController : function() {
		this.registerEvent([{
			elementId : 'systemS1CenterEngineCutoffButton',
			eventName : 'click',
			callback : SATURN_V.controller.MainComputer.systemS1ButtonController,
			callbackArguments : ['systemS1CenterEngineCutoffButton'],
			scope : SATURN_V.controller.MainComputer
		}, {
			elementId : 'systemS1ActionButton',
			eventName : 'click',
			callback : SATURN_V.controller.MainComputer.systemS1ButtonController,
			callbackArguments : ['systemS1ActionButton'],
			scope : SATURN_V.controller.MainComputer
		}]);
	},

	systemS2ButtonController : function() {
		this.registerEvent([{
			elementId : 'systemS2CenterEngineCutoffButton',
			eventName : 'click',
			callback : SATURN_V.controller.MainComputer.systemS2ButtonController,
			callbackArguments : ['systemS2CenterEngineCutoffButton'],
			scope : SATURN_V.controller.MainComputer
		}, {
			elementId : 'systemS2ActionButton',
			eventName : 'click',
			callback : SATURN_V.controller.MainComputer.systemS2ButtonController,
			callbackArguments : ['systemS2ActionButton'],
			scope : SATURN_V.controller.MainComputer
		}]);
	},

	systemS3ButtonController : function() {
		this.registerEvent([{
			elementId : 'systemS3Restart',
			eventName : 'click',
			callback : SATURN_V.controller.MainComputer.systemS3ButtonController,
			callbackArguments : ['systemS3Restart'],
			scope : SATURN_V.controller.MainComputer
		}, {
			elementId : 'systemS3ActionButton',
			eventName : 'click',
			callback : SATURN_V.controller.MainComputer.systemS3ButtonController,
			callbackArguments : ['systemS3ActionButton'],
			scope : SATURN_V.controller.MainComputer
		}]);
	},

	holddownArmsButtonController : function() {
		this.registerEvent([{
			elementId : 'holddownArmsButton',
			eventName : 'click',
			callback : SATURN_V.controller.MainComputer.holddownArmsButtonController,
			callbackArguments : ['holddownArmsButton'],
			scope : SATURN_V.controller.MainComputer
		}]);
	},

	internalGuidanceButtonController : function() {
		this.registerEvent([{
			elementId : 'internalGuidanceButton',
			eventName : 'click',
			callback : SATURN_V.controller.MainComputer.internalGuidanceButtonController,
			callbackArguments : ['internalGuidanceButton'],
			scope : SATURN_V.controller.MainComputer
		}]);
	},

	countdownButtonButtonController : function() {
		this.registerEvent([{
			elementId : 'countdownButton',
			eventName : 'click',
			callback : SATURN_V.controller.MainComputer.countdownButtonButtonController,
			callbackArguments : ['countdownButton'],
			scope : SATURN_V.controller.MainComputer
		}]);
	},

	onSaturnVLabelClick : function() {
		this.registerEvent([{
			elementId : 'saturnVconfig',
			eventName : 'click',
			callback : SATURN_V.controller.Network.getSaturnVRemoteAddress,
			callbackArguments : [true],
			scope : SATURN_V.controller.Network
		}]);
	},

	onMissionTimeLabelClick : function() {
		this.registerEvent([{
			elementId : 'voyager7_mission_time',
			eventName : 'click',
			callback : SATURN_V.controller.MainView.switchMissionTimeFormat,
			callbackArguments : [true],
			scope : SATURN_V.controller.MainView
		}]);
	}
});