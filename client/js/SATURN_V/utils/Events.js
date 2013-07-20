JSMVC.define('SATURN_V.utils.Events', {
	
	init : function() {
		this.onSaturnVLabelClick();
		this.showApolloSA514ControlPanel();
		this.showApolloLMControlPanel();
		this.showApolloCSMControlPanel();
		this.showHoustonMCControlPanel();
	},

	onSaturnVLabelClick : function() {
		document.getElementById('saturnVconfig').addEventListener('click', function() {
			SATURN_V.controller.Network.getSaturnVRemoteAddress(true);
		});
	},

	showApolloSA514ControlPanel : function() {
		document.getElementById('showApolloSA514ControlPanel_CSM').addEventListener('click', function() {
			SATURN_V.controller.MainView.showApolloSA514ControlPanel();
		});
		document.getElementById('showApolloSA514ControlPanel_LM').addEventListener('click', function() {
			SATURN_V.controller.MainView.showApolloSA514ControlPanel();
		});
		document.getElementById('showApolloSA514ControlPanel_MC').addEventListener('click', function() {
			SATURN_V.controller.MainView.showApolloSA514ControlPanel();
		});
	},

	showApolloLMControlPanel : function() {
		document.getElementById('showApolloLMControlPanel_CSM').addEventListener('click', function() {
			SATURN_V.controller.MainView.showApolloLMControlPanel();
		});
	},

	showApolloCSMControlPanel : function() {
		document.getElementById('showApolloCSMControlPanel_LM').addEventListener('click', function() {
			SATURN_V.controller.MainView.showApolloCSMControlPanel();
		});
		document.getElementById('showApolloCSMControlPanel_MC').addEventListener('click', function() {
			SATURN_V.controller.MainView.showApolloCSMControlPanel();
		});
		document.getElementById('showApolloCSMControlPanel_SA').addEventListener('click', function() {
			SATURN_V.controller.MainView.showApolloCSMControlPanel();
		});
	},

	showHoustonMCControlPanel : function() {
		document.getElementById('showHoustonMCControlPanel_SA').addEventListener('click', function() {
			SATURN_V.controller.MainView.showHoustonMCControlPanel();
		});
		
	}
});