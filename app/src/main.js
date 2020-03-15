import Vue from 'vue'
import App from './App.vue'
import router from './router'
import vuetify from './plugins/vuetify'

Vue.config.productionTip = false

Vue.use(require('vue-moment'));

new Vue({
	vuetify,
	router,
	data: {
		stats: null
	},
	render: h => h(App)
}).$mount('#app')
