import Vue from 'vue'
import VueRouter from 'vue-router'

const Dashboard = () => import('../views/Dashboard')
const Settings = () => import('../views/Settings')
//const Wifi = () => import(/* webpackChunkName: "wifi" */ '../views/Wifi')
//const Sandbox = () => import(/* webpackChunkName: "sandbox" */ '../views/Sandbox')


Vue.use(VueRouter);

export default new VueRouter({
    routes: [
        { path: '/', component: Dashboard },
        { path: '/settings', component: Settings, meta: { transitionName: 'slide' } },
        //{ path: '/wifi', component: Wifi },
        //{ path: '/sandbox', component: Sandbox },

        { path: '*', redirect: '/' }
    ],
});
