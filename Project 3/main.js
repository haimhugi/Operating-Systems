// NAME: Haim Hugi 207686650

const fs = require('fs');

//Get the parameter from run command


for (let index = 1; index < 6; index++) {
    var text = fs.readFileSync("./input" + index + ".txt").toString('utf-8');
    let inputArray = text.split('\n').toString().replace(/\n|\r/g, "").trim();
    inputArray = inputArray.split(',');
    for (let index = 0; index < inputArray.length; index++) {
        if (!isNumeric(inputArray[index])) {
            inputArray.splice(index);
        }
    }

    const answerFCFS = FCFS(inputArray); // V
    const answerLCFSNP = LCFSNP(inputArray); // V
    const answerLCFSP = LCFSP(inputArray); // V
    const answerRoundRobin = RoundRobin(inputArray); // V
    const answerSJF = SJF(inputArray); // V

    console.log(`FCFS: mean turnaround ${answerFCFS}`);
    console.log(`LCFS (NP): mean turnaround ${answerLCFSNP}`);
    console.log(`LCFS (P): mean turnaround ${answerLCFSP}`);
    console.log(`RR: mean turnaround ${answerRoundRobin}`);
    console.log(`SJF: mean turnaround ${answerSJF}` + "\n");
}



function FCFS(inputArray) {
    let numberOfProccess = inputArray[0];
    const enterTimeArray = [];
    const workTimeArray = [];
    for (let i = 1; i < inputArray.length; i++) {
        const element = inputArray[i];
        if (i % 2 === 1) {
            enterTimeArray.push(element);
        }
        if (i % 2 === 0) {
            workTimeArray.push(element);
        }
    }
    let whereNow = 1;
    let SumKolelTurnAround = 0;
    for (let i = 0; i < numberOfProccess; i++) {
        let waitingTimeCounter;
        let runningTimeCounter;
        let turnAroundSum;

        let min = Math.min(...enterTimeArray);
        if (whereNow < min) {
            whereNow = min;
        }
        let indexOfMinNumber = enterTimeArray.indexOf(min.toString());
        waitingTimeCounter = whereNow - parseInt(enterTimeArray[indexOfMinNumber]);
        whereNow += parseInt(workTimeArray[indexOfMinNumber]);
        runningTimeCounter = parseInt(workTimeArray[indexOfMinNumber]);
        turnAroundSum = parseInt(waitingTimeCounter) + runningTimeCounter;
        SumKolelTurnAround += turnAroundSum;
        enterTimeArray.splice(indexOfMinNumber, 1);
        workTimeArray.splice(indexOfMinNumber, 1);
    }
    return SumKolelTurnAround / numberOfProccess;
}

function LCFSNP(inputArray) {
    let numberOfProccess = inputArray[0];
    const enterTimeArray = [];
    const workTimeArray = [];
    for (let i = 1; i < inputArray.length; i++) {
        const element = inputArray[i];
        if (i % 2 === 1) {
            enterTimeArray.push(element);
        }
        if (i % 2 === 0) {
            workTimeArray.push(element);
        }
    }
    let CopyArray;
    let whereNow = Math.min(...enterTimeArray);
    let SumKolelTurnAround = 0;
    for (let i = 0; i < numberOfProccess; i++) {
        if (CopyArray != undefined) {
            if (whereNow < Math.min(...enterTimeArray))
                whereNow = Math.min(...enterTimeArray);
        }
        let waitingTimeCounter;
        let runningTimeCounter;
        let turnAroundSum;

        let max = Math.max(...enterTimeArray);
        let indexOfMaxNumber = enterTimeArray.indexOf(max.toString());
        let maxtemp = max;
        CopyArray = [...enterTimeArray];
        while (whereNow < maxtemp) {
            indexOfMaxNumber = CopyArray.indexOf(maxtemp.toString());
            CopyArray.splice(indexOfMaxNumber, 1);
            maxtemp = Math.max(...CopyArray);
        }
        indexOfMaxNumber = enterTimeArray.lastIndexOf(maxtemp.toString());
        waitingTimeCounter = whereNow - parseInt(enterTimeArray[indexOfMaxNumber]);
        whereNow += parseInt(workTimeArray[indexOfMaxNumber]);
        runningTimeCounter = parseInt(workTimeArray[indexOfMaxNumber]);
        turnAroundSum = parseInt(waitingTimeCounter) + runningTimeCounter;
        SumKolelTurnAround += turnAroundSum;
        enterTimeArray.splice(indexOfMaxNumber, 1);
        workTimeArray.splice(indexOfMaxNumber, 1);
    }
    return SumKolelTurnAround / numberOfProccess;

}

function LCFSP(inputArray) {


    let numberOfProccess = inputArray[0];
    const enterTimeArray = [];
    const workTimeArray = [];
    for (let i = 1; i < inputArray.length; i++) {
        const element = inputArray[i];
        if (i % 2 === 1) {
            enterTimeArray.push(element);
        }
        if (i % 2 === 0) {
            workTimeArray.push(element);
        }
    }


    let whereNow = 1;
    let SumKolelTurnAround = 0;

    let waitingTimeArray = [];
    for (let index = 0; index < numberOfProccess; index++) {
        waitingTimeArray[index] = 0;
    }
    let indexOfActive = 0;
    let activeProcesstTor = new Array(parseInt(numberOfProccess)).fill(-1);
    let counter = 0;
    let flag = 1;


    while (1) {

        counter = 0;
        for (let index = 0; index < workTimeArray.length; index++) {
            const element = workTimeArray[index];
            if (element == 0) {
                for (let z = 0; z < activeProcesstTor.length; z++) {
                    const element1 = activeProcesstTor[z];
                    if (element1 == index) {
                        activeProcesstTor[z] = -1;
                    }

                }
            }

        }

        flag = 1;
        if (indexOfActive == numberOfProccess) {
            indexOfActive = 0;
        }
        //Check If i on -1 in activeproccess if yes then i skip to the next
        while (activeProcesstTor[indexOfActive] === -1) {
            indexOfActive++;
            if (indexOfActive == numberOfProccess) {
                indexOfActive = 0;
            }
            if (counter == numberOfProccess - 1) {
                counter = 0;
                flag = 0;
                break;
            }
            counter++;
        }

        if (flag != 0) {
            //active tor not empty
            let CopyArray = [...enterTimeArray];
            let max = Math.max(...enterTimeArray);
            let indexOfMaxNumber = enterTimeArray.indexOf(max.toString());
            let maxtemp = max;

            while (whereNow < maxtemp + 1) {
                indexOfMaxNumber = CopyArray.indexOf(maxtemp.toString());
                CopyArray.splice(indexOfMaxNumber, 1);
                maxtemp = Math.max(...CopyArray);
            }

            indexOfActive = enterTimeArray.lastIndexOf(maxtemp.toString());

            if (indexOfActive != undefined) {
                if (workTimeArray[indexOfActive] == 0) {
                    SumKolelTurnAround += whereNow - parseInt(enterTimeArray[indexOfActive]);
                    enterTimeArray[indexOfActive] = -1;
                    for (let j = 0; j < activeProcesstTor.length; j++) {
                        const element = activeProcesstTor[j];
                        if (element == indexOfActive) {
                            activeProcesstTor[j] = -1;
                        }
                    }
                }

                workTimeArray[indexOfActive]--;
                if (workTimeArray[indexOfActive] == 0) {
                    SumKolelTurnAround += whereNow - parseInt(enterTimeArray[indexOfActive]);
                    enterTimeArray[indexOfActive] = -1;
                    for (let j = 0; j < activeProcesstTor.length; j++) {
                        const element = activeProcesstTor[j];
                        if (element == indexOfActive) {
                            activeProcesstTor[j] = -1;
                        }
                    }
                }
            }
        }

        for (let i = 0; i < enterTimeArray.length; i++) {
            const item = parseInt(enterTimeArray[i]);
            if (item === whereNow) {
                let temp = activeProcesstTor[indexOfActive];
                activeProcesstTor.splice(indexOfActive, 0, i);
                if (workTimeArray[i] == 0) {
                    SumKolelTurnAround++;
                }
                for (let i = activeProcesstTor.length - 1; i < activeProcesstTor.length; i--) {
                    const element = activeProcesstTor[i];
                    if (element === -1) {
                        activeProcesstTor.splice(i, 1);
                        //Check if break get out from the for loop
                        break;
                    }
                }

            }
        }
        let done = true;
        for (let i = 0; i < workTimeArray.length; i++) {
            const element = workTimeArray[i];
            if (element != 0) {
                done = false;
                break;
            }
        }
        if (done) {
            break;
        }
        whereNow++;
    }
    return SumKolelTurnAround / numberOfProccess;

}

function RoundRobin(inputArray) {
    let numberOfProccess = inputArray[0];
    const enterTimeArray = [];
    const workTimeArray = [];
    for (let i = 1; i < inputArray.length; i++) {
        const element = inputArray[i];
        if (i % 2 === 1) {
            enterTimeArray.push(element);
        }
        if (i % 2 === 0) {
            workTimeArray.push(element);
        }
    }


    let timeQuantom = 2;
    let whereNow = 1;
    let SumKolelTurnAround = 0;

    // let sumWorkingTimeArray = [...workTimeArray];
    let waitingTimeArray = [];
    for (let index = 0; index < numberOfProccess; index++) {
        waitingTimeArray[index] = 0;
    }
    let indexOfActive = 0;

    let activeProcesstTor = new Array(parseInt(numberOfProccess)).fill(-1);
    let counter = 0;
    let flag = 1;


    while (1) {


        flag = 1;
        if (indexOfActive == numberOfProccess) {
            indexOfActive = 0;
        }
        //Check If i on -1 in activeproccess if yes then i skip to the next
        while (activeProcesstTor[indexOfActive] === -1) {
            indexOfActive++;
            if (indexOfActive == numberOfProccess) {
                indexOfActive = 0;
            }
            if (counter == numberOfProccess - 1) {
                counter = 0;
                flag = 0;
                break;
            }
            counter++;
        }

        if (flag != 0) {
            //active tor not empty
            if (workTimeArray[activeProcesstTor[indexOfActive]] == 0) {
                SumKolelTurnAround += whereNow - enterTimeArray[activeProcesstTor[indexOfActive]];
                activeProcesstTor[indexOfActive] = -1;
                timeQuantom = 2;
                indexOfActive++;
                whereNow++;
                continue;
            }
            workTimeArray[activeProcesstTor[indexOfActive]]--;
            if (workTimeArray[activeProcesstTor[indexOfActive]] == 0) {
                SumKolelTurnAround += whereNow - enterTimeArray[activeProcesstTor[indexOfActive]];
                activeProcesstTor[indexOfActive] = -1;
                timeQuantom = 2;
                indexOfActive++;
                whereNow++;
                continue;
            }


            timeQuantom--;


        }

        for (let i = 0; i < enterTimeArray.length; i++) {
            const item = parseInt(enterTimeArray[i]);
            if (item === whereNow) {
                let temp = activeProcesstTor[indexOfActive];

                activeProcesstTor.splice(indexOfActive, 0, i);
                for (let i = activeProcesstTor.length - 1; i < activeProcesstTor.length; i--) {
                    const element = activeProcesstTor[i];
                    if (element === -1) {
                        activeProcesstTor.splice(i, 1);
                        //Check if break get out from the for loop
                        break;
                    }
                }
                if (temp != -1) {
                    for (let i = 0; i < activeProcesstTor.length; i++) {
                        const element = activeProcesstTor[i];
                        if (element == temp) {
                            indexOfActive = i;
                            break;
                        }
                    }
                }

            }
        }

        if (timeQuantom === 0) {
            indexOfActive++;
            if (indexOfActive == numberOfProccess) {
                indexOfActive = 0;
            }
            let count = 0;

            while (activeProcesstTor[indexOfActive] == -1) {
                if (count > numberOfProccess) {
                    break;
                }
                indexOfActive++;
                if (indexOfActive == numberOfProccess) {
                    indexOfActive = 0;
                }
                count++;
            }

            timeQuantom = 2;
            whereNow++;
            continue;
        }

        let done = true;
        for (let i = 0; i < workTimeArray.length; i++) {
            const element = workTimeArray[i];
            if (element != 0) {
                done = false;
                break;
            }
        }
        if (done) {
            break;
        }

        whereNow++;

    }
    return SumKolelTurnAround / numberOfProccess;
}


function SJF(inputArray) {

    let numberOfProccess = inputArray[0];
    const enterTimeArray = [];
    const workTimeArray = [];
    for (let i = 1; i < inputArray.length; i++) {
        const element = inputArray[i];
        if (i % 2 === 1) {
            enterTimeArray.push(element);
        }
        if (i % 2 === 0) {
            workTimeArray.push(element);
        }
    }


    let whereNow = 1;
    let SumKolelTurnAround = 0;

    let waitingTimeArray = [];
    for (let index = 0; index < numberOfProccess; index++) {
        waitingTimeArray[index] = 0;
    }
    let indexOfActive = 0;
    let activeProcesstTor = new Array(parseInt(numberOfProccess)).fill(-1);
    let counter = 0;
    let flag = 1;
    let activeNow;

    while (1) {


        for (let index = 0; index < workTimeArray.length; index++) {
            const element = workTimeArray[index];
            if (element == 0) {
                for (let z = 0; z < activeProcesstTor.length; z++) {
                    const element1 = activeProcesstTor[z];
                    if (element1 == index) {
                        activeProcesstTor[z] = -1;
                    }

                }
            }

        }

        flag = 1;
        if (indexOfActive == numberOfProccess) {
            indexOfActive = 0;
        }
        //Check If i on -1 in activeproccess if yes then i skip to the next
        while (activeProcesstTor[indexOfActive] === -1) {
            indexOfActive++;
            if (indexOfActive == numberOfProccess) {
                indexOfActive = 0;
            }
            if (counter == numberOfProccess - 1) {
                counter = 0;
                flag = 0;
                break;
            }
            counter++;
        }

        if (flag != 0) {
            //active tor not empty

            let minTemp = Infinity;
            let indexOfMinWorking;
            for (let i = 0; i < workTimeArray.length; i++) {
                const element = parseInt(workTimeArray[i]);
                if (element == 0) {
                    continue;
                }
                if (element < minTemp) {
                    activeNow = 0;

                    for (let index = 0; index < activeProcesstTor.length; index++) {
                        const element = activeProcesstTor[index];
                        if (i == element) {
                            activeNow = 1;
                            break;
                        }
                    }

                    if (activeNow) {
                        minTemp = element
                        indexOfMinWorking = i;
                    }
                }
                if (element == minTemp) {
                    //check who came in first
                    if (enterTimeArray[i] < enterTimeArray[indexOfMinWorking]) {
                        minTemp = element
                        indexOfMinWorking = i;
                    }
                }
            }
            if (indexOfMinWorking != undefined) {
                if (workTimeArray[indexOfMinWorking] == 0) {
                    SumKolelTurnAround += whereNow - parseInt(enterTimeArray[indexOfMinWorking]);
                    for (let j = 0; j < activeProcesstTor.length; j++) {
                        const element = activeProcesstTor[j];
                        if (element == indexOfMinWorking) {
                            activeProcesstTor[j] = -1;
                        }

                    }
                }

                workTimeArray[indexOfMinWorking]--;
                if (workTimeArray[indexOfMinWorking] == 0) {
                    SumKolelTurnAround += whereNow - parseInt(enterTimeArray[indexOfMinWorking]);
                    for (let j = 0; j < activeProcesstTor.length; j++) {
                        const element = activeProcesstTor[j];
                        if (element == indexOfMinWorking) {
                            activeProcesstTor[j] = -1;
                        }

                    }
                }


            }
        }

        for (let i = 0; i < enterTimeArray.length; i++) {
            const item = parseInt(enterTimeArray[i]);
            if (item === whereNow) {
                let temp = activeProcesstTor[indexOfActive];
                activeProcesstTor.splice(indexOfActive, 0, i);
                if (workTimeArray[i] == 0) {
                    SumKolelTurnAround++;
                }
                for (let i = activeProcesstTor.length - 1; i < activeProcesstTor.length; i--) {
                    const element = activeProcesstTor[i];
                    if (element === -1) {
                        activeProcesstTor.splice(i, 1);
                        //Check if break get out from the for loop
                        break;
                    }
                }

            }
        }


        let done = true;
        for (let i = 0; i < workTimeArray.length; i++) {
            const element = workTimeArray[i];
            if (element != 0) {
                done = false;
                break;
            }
        }
        if (done) {
            break;
        }
        whereNow++;
    }
    return SumKolelTurnAround / numberOfProccess;

}

function isNumeric(str) {
    if (typeof str != "string") return false
    return !isNaN(str) &&
        !isNaN(parseFloat(str))
}





