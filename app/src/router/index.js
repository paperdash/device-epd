import Vue from 'vue'
import VueRouter from 'vue-router'

const Dashboard = () => import('../views/Dashboard')
const Device = () => import('../views/Device')
const Playlist = () => import('../views/Playlist')
const Wifi = () => import('../views/Wifi')
const Weather = () => import('../views/Weather')
const System = () => import('../views/System')

const SetupStart = () => import('../views/Setup/Start')
const SetupCountry = () => import('../views/Setup/Country')
const SetupWifi = () => import('../views/Setup/Wifi')
const SetupWeather = () => import('../views/Setup/Weather')
const SetupName = () => import('../views/Setup/Name')
const SetupAppearance = () => import('../views/Setup/Appearance')
const SetupDone = () => import('../views/Setup/Done')

Vue.use(VueRouter)

export default new VueRouter({
    routes: [
      { path: '/', component: Dashboard },
      { path: '/device', component: Device, meta: { transitionName: 'slide' } },
      { path: '/playlist', component: Playlist, meta: { transitionName: 'slide' } },
      { path: '/wifi', component: Wifi, meta: { transitionName: 'slide' } },
      { path: '/weather', component: Weather, meta: { transitionName: 'slide' } },
      { path: '/system', component: System, meta: { transitionName: 'slide' } },

      // setup wizard
      { path: '/setup', redirect: '/setup/start' },
      { path: '/setup/start', component: SetupStart, meta: { transitionName: 'slide', layout: 'setup' } },
      { path: '/setup/country', component: SetupCountry, meta: { transitionName: 'slide', layout: 'setup' } },
      { path: '/setup/wifi', component: SetupWifi, meta: { transitionName: 'slide', layout: 'setup' } },
      { path: '/setup/weather', component: SetupWeather, meta: { transitionName: 'slide', layout: 'setup' } },
      { path: '/setup/name', component: SetupName, meta: { transitionName: 'slide', layout: 'setup' } },
      { path: '/setup/appearance', component: SetupAppearance, meta: { transitionName: 'slide', layout: 'setup' } },
      { path: '/setup/done', component: SetupDone, meta: { transitionName: 'slide', layout: 'setup' } },

      { path: '*', redirect: '/' },
    ],
    scrollBehavior (to, from, savedPosition) {
      if (savedPosition) {
        return savedPosition
      } else {
        return { x: 0, y: 0 }
      }
    },
})
