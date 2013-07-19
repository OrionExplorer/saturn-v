JSMVC.application('SATURN_V', {
	controllers : [
		'Network',
		'ControlPanel',
		'Events'
	],
	requires : [
		'SATURN_V.utils.Shared',
		'SATURN_V.utils.Frontend'
	],
	appPath : 'js/',

	launch : function() {

	}
});