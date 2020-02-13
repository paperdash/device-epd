import Vue from 'vue'
import VueRouter from 'vue-router'

const Dashboard = () => import(/* webpackChunkName: "dashboard" */ '../views/Dashboard')
const Settings = () => import(/* webpackChunkName: "settings" */ '../views/Settings')
const Sandbox = () => import(/* webpackChunkName: "sandbox" */ '../views/Sandbox')


Vue.use(VueRouter);

export default new VueRouter({
    routes: [
        { path: '/', component: Dashboard },
        { path: '/settings', component: Settings },
        { path: '/sandbox', component: Sandbox },

        { path: '*', redirect: '/' }
    ],
});
