let currentMap = null;
let players = [];
let mapImg = null;
let mapDiv = null;
let mapData = null;
let artificialScale = 1;
let isAnimating = false;
let averageTimeMs = 0;
let currentAverageCalc = 0;
let currentDataSize = 0;
let USE_FAKE_DATA = false;
var dataLog = [];
let connectionTimeRef = null;
/*
player object structure;
htmlRef; html element reference
name; name of player
data; position [x, y, z], angle: radians
lastUpdate: time since last update
*/
var lastReceived = new Date();
const initConnection = async () => {
    if (!USE_FAKE_DATA){
        const socket = new WebSocket("ws://188.24.175.93:22006/foo");
        
        socket.onopen = () => {
            console.info('Connected to websocket.');
        }
        socket.onclose = () => {
            console.info("Disconnected from websocket.");
        }
        socket.addEventListener("message", async (event) => {
           
            var obj = await event.data.text();


            const diffInMs = new Date().getTime() - lastReceived.getTime();

            if (averageTimeMs == 0)
                averageTimeMs = diffInMs;

            connectionTimeRef.innerText = averageTimeMs + ' MS';

            if (currentAverageCalc >= 10)
            {
                averageTimeMs = currentAverageCalc / currentDataSize;
                console.info('averageTimeMs ' + averageTimeMs);
                currentDataSize = 0;
                currentAverageCalc = 0;
            }else{
                currentDataSize++;
                currentAverageCalc += diffInMs;
            }

            console.log('dif since last received data ' +  diffInMs)
            lastReceived = new Date();
            //console.log("Message from server ", obj);
            await updateRadarData(JSON.parse(obj));
            
          })
        }else{
            var response = await fetch('fakeData.json');
            dataLog = await response.json();
            averageTimeMs = 100;
            var count = 0;
            setInterval(async () => {
                if (count >= dataLog.length){
                    count = 0;
                }
                await updateRadarData(dataLog[count]);
                count++;
                
            }, 100);
        }

}

const setCurrentMap = (map) => currentMap = map;

const fetchMapData = async () => {


    var response = await fetch('data/' + currentMap + '.json');

    return response.json();
}
/*
{"map":"de_mirage","players":[{"data":{"index":0,"position":{"x":-1485.0078125,"y":-2100.45703125},"team":2}}]}
*/
    
const sampleRadarUpdate = {
    map: 'de_mirage',
    players: [{
        index: 3,
        name: 'Denis',
        data: {
            position: { x: -3530, y: 2013 },
            angle: 0.3,
            team: 'CT'
        }
    }]
}

const updatePlayer = (index, data) => {
    if (players[index] == null) {
        createPlayer(index, data);
        return;
    }


    players[index].data = data;
    //if(isAnimating) return;
    const div = players[index].htmlRef;

    const pos = getRadarPositionOfCoords(data.position);
    const htmlRef = players[index].htmlRef;

    var bounding = htmlRef.getBoundingClientRect();

  //  console.log('crostps ' + JSON.stringify(pos));
    //div.style.left = pos.x  + '%';
    //div.style.top = pos.y + '%';
    var positionInfo = mapImg.getBoundingClientRect();
    var translateX = positionInfo.width * pos.x;
    var translateY = positionInfo.height * pos.y;
  //  console.log('translateX ' + translateX)

    translateX -= bounding.width * 0.5;
    translateY -= bounding.height * 0.5;

    translateX /= artificialScale;
    translateY /= artificialScale;

    //div.style.top = translateY + 'px';
   // div.style.left = translateX + 'px';

    div.style.transition = `transform ${averageTimeMs}ms linear`;
    div.style.transform = `translate(${translateX}px, ${translateY}px)`;
}

const onMapChange = async () => {
    console.info('changed map level to ' + currentMap);
    mapImg.src = 'images/' + currentMap + '_radar_psd.png';
    mapImg.classList.add('radar__image');
    mapData = await fetchMapData();

    console.log('mapData cristos ' + JSON.stringify(mapData))
    console.log(mapData);

}

const changeArtificialScale = (scale) => {
    artificialScale = scale;
    mapDiv.style.transform = `scale(${scale})`
}
const getRadarPositionOfCoords = (coords) => {
    const mapX = (coords.x - mapData.pos_x) / mapData.scale / 1024.0;
    const mapY = (coords.y - mapData.pos_y) / mapData.scale * -1.0 / 1024.0;

    

   // console.log(mapData)
    return { x: mapX , y: mapY };
}
const createPlayer = (index, data) => {


    const div = document.createElement('div');

    div.classList.add('CT_Player');
    const pos = getRadarPositionOfCoords(data.position);

    console.log('pos ' +JSON.stringify(pos));

    players[index] = { htmlRef: div, name: 'unk' + index, data: data, lastUpdate: new Date() };


    mapDiv.appendChild(players[index].htmlRef);
}

const updateRadarData = async (radarData) => {
    console.log(radarData);
    if (radarData.map == null) return;
    if (radarData.map !== currentMap) {
        currentMap = radarData.map;
        await onMapChange();
    }

   
    console.log('mata');
    if (radarData.players == null) return;

    radarData.players.forEach(player => {
        updatePlayer(player.index, player.data);
    })

}
const DOMContentLoaded = async (ev) => {
    mapDiv = document.createElement('div');
    mapImg = document.createElement('img');
    
    mapDiv.classList.add('radar__container');
    mapImg.classList.add('radar__image');

    mapDiv.appendChild(mapImg);

    document.body.appendChild(mapDiv);


    connectionTimeRef = document.createElement('div');
    connectionTimeRef.classList.add('radar__connection');

    mapDiv.appendChild(connectionTimeRef);

    changeArtificialScale(1)


    await initConnection();
    
}







document.addEventListener('DOMContentLoaded', DOMContentLoaded);