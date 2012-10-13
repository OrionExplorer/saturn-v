var _computerProgram = function() {
    this.running = false;
    this.startTime = false;
    this.running_time = 0;
    this.name = undefined;

    this.getPossibleAction = function() {
        return this.running ? 'STOP' : 'START';
    }

    this.start = function() {
        var currentDate = new Date();
        this.startTime = currentDate.getTime();
        delete currentDate;
        this.running = true;
    }

    this.stop = function() {
        this.running = false;
        this.running_time = 0;
    }
}