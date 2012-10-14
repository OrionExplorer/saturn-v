var lastComputerMessage = '';

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
	LET : 9
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
    
    if(json.computer_message != lastComputerMessage) {
        updateInformation(json.computer_message);
    }
    updateHUD('yawButton', (json.yaw_program_engaged == 1 ? 'STOP' : 'START' ));
    updateHUD('rollButton', (json.roll_program_engaged == 1 ? 'STOP' : 'START' ));
    updateHUD('pitchButton', (json.pitch_program_engaged == 1 ? 'STOP' : 'START' ));
    updateHUD('voyager7_pitch', Math.round(json.pitch)+'° / '+json.dest_pitch+'°');
    updateHUD('voyager7_rollAzimuth', Math.round(90-json.roll)+'° / '+json.dest_roll+'°');
    updateHUD('voyager7_yaw', Math.round(json.yaw*10)/10+'°');
    updateHUD('voyager7_mainEngine', (json.main_engine_engaged == 1 ? 'ON' : 'OFF' ));

    updateHUD('voyager7_s1_fuel', Math.round(json.s_ic_fuel)+' KG');
    updateHUD('voyager7_s1_burnTime', Math.round(json.s_ic_burn_time)+' S');
    updateHUD('voyager7_s1_status', (json.s_ic_attached == 1 ? 'ATTACHED' : 'STAGED'));
    updateHUD('voyager7_s1_thrust', Math.round(json.s_ic_thrust)+' N');

    updateHUD('voyager7_s2_fuel', Math.round(json.s_ii_fuel)+' KG');
    updateHUD('voyager7_s2_burnTime', Math.round(json.s_ii_burn_time)+' S');
    updateHUD('voyager7_s2_status', (json.s_ii_attached == 1 ? 'ATTACHED' : 'STAGED'));
    updateHUD('voyager7_s2_thrust', Math.round(json.s_ii_thrust)+' N');

    updateHUD('voyager7_s3_fuel', Math.round(json.s_ivb_fuel)+' KG');
    updateHUD('voyager7_s3_burnTime', Math.round(json.s_ivb_burn_time)+' S');
    updateHUD('voyager7_s3_status', (json.s_ivb_attached == 1 ? 'ATTACHED' : 'STAGED'));
    updateHUD('voyager7_s3_thrust', Math.round(json.s_ivb_thrust)+' N');
    
    updateHUD('voyager7_mission_time', secondsToHms(json.mission_time));
    updateHUD('voyager7_velocity', Math.round(json.last_velocity) + ' M/S');
    updateHUD('voyager7_verticalVelocity', Math.round(json.current_vertical_velocity) + ' M/S');
    updateHUD('voyager7_horizontalVelocity', Math.round(json.current_horizontal_velocity) + ' M/S');
    updateHUD('voyager7_totalMass', Math.round(json.total_mass) + ' KG');
    updateHUD('voyager7_acceleration', Math.round(json.current_acceleration*10)/10 + ' M/S');
    updateHUD('voyager7_gForce', json.current_gforce + ' G');
    updateHUD('voyager7_dynamicPressure', Math.round(json.current_dynamic_pressure)/10+' KG/M²');

    if(json.current_altitude <= 1000) {
        updateHUD('voyager7_altitude', Math.round(json.current_altitude) + ' M');
    } else if(json.current_altitude > 1000) {
        updateHUD('voyager7_altitude', Math.round(json.current_altitude/100)/10 + ' KM');
    }

    if(json.total_distance <= 1000) {
        updateHUD('voyager7_distance', Math.round(json.total_distance) + ' M');
    } else {
        updateHUD('voyager7_distance', Math.round(json.total_distance)/1000 + ' KM');
    }
    updateHUD('voyager7_thrust', json.current_thrust+' %');

    if(json.mission_time > 0) {
        enableThrustChange();
    }

    if(json.internal_guidance_engaged == 0) {
        updateHUD('mainEngineEngageButton', 'ENGAGE');//, 2000, 'enableEl("'+id+'")');
    } else {
        updateHUD('mainEngineEngageButton', 'DISENGAGE');//, 2000, 'enableEl("'+id+'")');
    }

    if(json.main_engine_engaged == 0) {
        updateHUD('systemAPSButton', 'ENGAGE');
    } else {
        updateHUD('systemAPSButton', 'DISENGAGE');
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