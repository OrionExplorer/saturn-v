JSMVC.application('SATURN_V', {
	controllers : [
		'Network',
		'MainView',
		'MainComputer'
	],
	requires : [
		'SATURN_V.utils.Shared',
		'SATURN_V.utils.Frontend'
	],
	appPath : 'js/'
});