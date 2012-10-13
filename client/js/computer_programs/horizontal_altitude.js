var horizontalAltitudeProgram = new _computerProgram();
horizontalAltitudeProgram.name = 'HORIZONTAL ALTITUDE PROGRAM';
horizontalAltitudeProgram.steps = [{
    id : 0,
    done : true,
    duration : 0
}, {
    id : 1,
    done : false,
    duration : 80
}, {
    id : 2,
    done : false,
    duration : 1
}];
horizontalAltitudeProgram.currentStep = 0;

var horizontalAltitudeEstablished = 1;

function getHorizontalAltitudeStatus() {
    if(horizontalAltitudeProgram.running && horizontalAltitudeEstablished == 0) {
        return 'PROCESSING...';
    } else {
        switch(horizontalAltitudeEstablished) {
            case 0 : return 'SEARCHING...'; break;
            case 1 : return 'ESTABLISHED'; break;
        }
    }
}