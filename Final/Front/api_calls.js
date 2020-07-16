const axios = require('axios')

const logError = error => console.log(error.response)

axios.defaults.baseURL = "https://yj7gpz.internetofthings.ibmcloud.com/api/v0002/"

const getTemperature = () => axios.get("device/types/Sensor/devices/TEMPERATURE1/state/5f08d586ac00880008af6ac8", {
    headers: {
        accept: "application/json"
    },
    auth: {
        username: "a-yj7gpz-huxbxokmhr",
        password: "e0y()oCB1k)Mes8UzY"
    }
}).then(
    res => console.log(res.data)
).catch(logError)

const getSwitchStatus = () => axios.get("device/types/Switch/devices/LIGHTSWITCH1/state/5f0a356dac00880008af6dfb", {
    headers: {
        accept: "application/json"
    },
    auth: {
        username: "a-yj7gpz-huxbxokmhr",
        password: "e0y()oCB1k)Mes8UzY"
    }
}).then(
    res => console.log(res.data)
).catch(logError)

const toggleSwitchStatus = () => axios.post("https://yj7gpz.messaging.internetofthings.ibmcloud.com/api/v0002/application/types/Switch/devices/LIGHTSWITCH1/commands/switch_request", {
    request: "toggle"
}, {
    headers: {
        accept: "application/json"
    },
    auth: {
        username: "a-yj7gpz-huxbxokmhr",
        password: "e0y()oCB1k)Mes8UzY"
    }
}).then(
    res => console.log(res.data)
).catch(logError)

getTemperature()
toggleSwitchStatus()
getSwitchStatus()