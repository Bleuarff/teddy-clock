'use strict'

function submit(){

}

function setTime(){
  var d = new Date(),
      time = `${d.getHours().toString().padStart(2, '0')}:${d.getMinutes().toString().padStart(2, '0')}`

  document.getElementById('current-time').innerText = time
}

setTime()
setInterval(setTime, 15000)

// TODO: query server for current times
