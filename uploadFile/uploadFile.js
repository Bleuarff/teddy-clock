'use strict'

/*******************************************************************************

Sends data through serial port.
Usage: node uploadFile.js FILE

*******************************************************************************/

const SerialPort = require('serialport'),
      fs = require('fs'),
      PORT = '/dev/ttyUSB0',
      PAGE_SIZE = 32,
      START_MARKER = 0x02,
      END_MARKER = 0x03;

const filePath = process.argv[2],
      content = fs.readFileSync(filePath, null)

void async function main(){

  var port = new SerialPort(PORT, {
    baudRate: 9600
  })

  await new Promise(resolve => {
    port.on('open', resolve)
  })

  console.log(`port ${PORT} open`)
  // Open errors will be emitted as an error event
  port.on('error', (err) => {
    console.error(err.stack)
    port.close()
    process.exit()
  })

  port.on('data', (data) => {
    var res = data.toString('ascii')
    process.stdout.write(res)

    // arduino responds with number of bytes, ended by a newline
    if (res.endsWith('\n')){
      port.close()
    }
  });

  // wait for arduino to open port, etc. It doesn't listen if you don't wait enough
  await new Promise(resolve => {setTimeout(resolve, 2000)})
  console.log(`Sending ${content.length} bytes`)

  try{
    // send start marker byte
    await port.write([START_MARKER], 'hex')
    console.log('---- STX')
  }
  catch(err){
    console.error(err.stack)
  }

  // send content bytes, by bursts of PAGE_SIZE
  for(let i = 0, c = 0, p = 0; i < content.length; i++){
    port.write([content[i]])
    // console.log(content[i])
    c++

    if (c === PAGE_SIZE){
      await new Promise(resolve => {
        let progress = (++p * PAGE_SIZE) / content.length * 100
        console.log(`---- ${progress.toFixed(1)}% ----`)
        setTimeout(resolve, 120)
      })
      c = 0
    }
  }

  // send end marker byte
  port.write([END_MARKER], 'hex')
  console.log('---- ETX')

  await new Promise(resolve => {
    port.drain(resolve)
  })
  console.log('     DONE')
}()
