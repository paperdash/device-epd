import Vue from 'vue'
import Vuex from 'vuex'
import axios from 'axios'
// import device from '@/api/device'

Vue.use(Vuex)

const store = new Vuex.Store({
  state: {
    stats: {},
    settings: {},
    notifications: {},
  },
  mutations: {
    setStats (state, payload) {
      state.stats = payload
    },
    setSettings (state, payload) {
      state.settings = payload
    },

    /*
    setSensors (state, payload) {
      state.sensors = payload
    },
    updateSensor (state, payload) {
      // update sensor
      const i = state.sensors.findIndex(item => item.id === payload.id)
      if (i >= 0) {
        state.sensors[i].temperature = payload.temperature
        state.sensors[i].humidity = payload.humidity
        state.sensors[i].last_update = payload.last_update

        if (payload.label) {
          state.sensors[i].label = payload.label
        }

        // update state
        state.sensors = [
          ...state.sensors,
        ]
      }
    },
    deleteSensor (state, id) {
      // const i = state.sensors.findIndex(item => item.id === id)
      state.sensors = state.sensors.filter(item => item.id !== id)
    },
    setPushUpdate (state, enable) {
      state.pushUpdate = enable
    },
    addSensorHistory (state, payload) {
      state.sensorHistory.push(payload)
      if (state.sensorHistory.length > 20) {
        state.sensorHistory = state.sensorHistory.slice(1)
      }
    },
    notification (state, payload) {
      state.notifications = payload
    },
     */
  },
  actions: {
    async loadStats ({ commit }) {
      try {
        const response = await axios.get('/stats')
        commit('setStats', response.data)
      } catch (error) {
        commit('setStats', {})
      }
    },
    async loadSettings ({ commit }) {
      try {
        const response = await axios.get('/api/settings')
        commit('setSettings', response.data)
      } catch (error) {
        commit('setSettings', {})
      }
    },
    /*
    async getSensors ({ commit }) {
      try {
        const response = await axios.get('/api/sensors')
        commit('setSensors', response.data)
      } catch (error) {
        commit('setSensors', [])
      }
    },
    async putSensor ({ commit }, [id, sensor]) {
      try {
        await axios.put('/api/sensor/' + id, {
          label: sensor.label,
        })
        commit('updateSensor', sensor)
      } catch (error) {
        console.warn(error)
      }
    },
    async deleteSensor ({ commit }, id) {
      try {
        await axios.delete('/api/sensor/' + id)
        commit('deleteSensor', id)
        commit('notification', 'sensor #' + id + ' deleted')
      } catch (error) {
        console.warn(error)
      }
    },

     */
  },
  getters: {
    /*
    isAuthenticated(state) {
      return state.user !== null && state.user !== undefined;
    }
     */
  },
})

// sensor push data
/*
const connection = new WebSocket('ws://' + window.location.host + '/ws')
connection.onmessage = (message) => {
  const log = JSON.parse(message.data)
  log.last_update = new Date()

  store.commit('updateSensor', log)
  store.commit('addSensorHistory', log)
  store.commit('notification', log)
}

store.watch(
  state => state.pushUpdate,
  (value) => {
    if (value) {
      console.info('TODO:: enable websocket')
    } else {
      console.info('TODO:: disable websocket')
    }
  },
)
*/
export default store
