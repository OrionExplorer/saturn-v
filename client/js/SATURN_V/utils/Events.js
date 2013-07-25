JSMVC.define('SATURN_V.utils.Events', {
	
	init : function() {
		this.onSaturnVLabelClick();
		this.showApolloSA514ControlPanel();
		this.showApolloLMControlPanel();
		this.showApolloCSMControlPanel();
		this.showHoustonMCControlPanel();

		this.countdownButtonButtonController();
		this.holddownArmsButtonController();
		this.internalGuidanceButtonController();

		this.mainEngineEngageButtonController();
		this.mainEngineThrustButtonController();
		this.systemS1ButtonController();
		this.systemS2ButtonController();
		this.systemS3ButtonController();
		this.pitchRollYawButtonController();

		this.autoPilotButtonController();
		this.chatInputController();
	},

	registerEvent : function(cfg) {
		var i = 0,
			listener = function(index) {
				document.getElementById(cfg[index].elementId).addEventListener(cfg[index].eventName, function(event) {
					cfg[index].callback.call(cfg[index].scope, cfg[index].callbackArguments ? cfg[index].callbackArguments : event);
				});
		};

		for(i = 0; i < cfg.length; i++) {
			listener(i);
		}
	},

	chatInputController : function() {
		this.registerEvent([{
			elementId : 'chat-input',
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

	mainEngineEngageButtonController : function() {
		this.registerEvent([{
			elementId : 'mainEngineEngageButton',
			eventName : 'click',
			callback : SATURN_V.controller.MainComputer.mainEngineEngageButtonController,
			callbackArguments : ['mainEngineEngageButton'],
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

	showApolloSA514ControlPanel : function() {
		this.registerEvent([{
			elementId : 'showApolloSA514ControlPanel_CSM',
			eventName : 'click',
			callback : SATURN_V.controller.MainView.showApolloSA514ControlPanel,
			callbackArguments : [],
			scope : SATURN_V.controller.MainView
		}, {
			elementId : 'showApolloSA514ControlPanel_LM',
			eventName : 'click',
			callback : SATURN_V.controller.MainView.showApolloSA514ControlPanel,
			callbackArguments : [],
			scope : SATURN_V.controller.MainView
		}, {
			elementId : 'showApolloSA514ControlPanel_MC',
			eventName : 'click',
			callback : SATURN_V.controller.MainView.showApolloSA514ControlPanel,
			callbackArguments : [],
			scope : SATURN_V.controller.MainView
		}]);
	},

	showApolloLMControlPanel : function() {
		this.registerEvent([{
			elementId : 'showApolloLMControlPanel_CSM',
			eventName : 'click',
			callback : SATURN_V.controller.MainView.showApolloLMControlPanel,
			callbackArguments : [],
			scope :SATURN_V.controller.MainView
		}]);
	},

	showApolloCSMControlPanel : function() {
		this.registerEvent([{
			elementId : 'showApolloCSMControlPanel_LM',
			eventName : 'click',
			callback : SATURN_V.controller.MainView.showApolloCSMControlPanel,
			callbackArguments : [],
			scope : SATURN_V.controller.MainView
		}, {
			elementId : 'showApolloCSMControlPanel_MC',
			eventName : 'click',
			callback : SATURN_V.controller.MainView.showApolloCSMControlPanel,
			callbackArguments : [],
			scope : SATURN_V.controller.MainView
		}, {
			elementId : 'showApolloCSMControlPanel_SA',
			eventName : 'click',
			callback : SATURN_V.controller.MainView.showApolloCSMControlPanel,
			callbackArguments : [],
			scope : SATURN_V.controller.MainView
		}]);
	},

	showHoustonMCControlPanel : function() {
		this.registerEvent([{
			elementId : 'showHoustonMCControlPanel_SA',
			eventName : 'click',
			callback : SATURN_V.controller.MainView.showHoustonMCControlPanel,
			callbackArguments : [],
			scope :SATURN_V.controller.MainView
		}]);		
	}
});