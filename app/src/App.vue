<template>
    <v-app _style="background: #e2e2e2">
		<template v-if="isLoading">
            <v-overlay :absolute="true" :value="true">
                <v-progress-circular indeterminate size="64"></v-progress-circular>
            </v-overlay>
        </template>

		<template v-else>
<!--
			<v-navigation-drawer
				v-model="drawer"
				app
			>
			</v-navigation-drawer>
			-->

			<v-system-bar app dark color="primary">
				<span>paperdash.io</span>
				<v-spacer></v-spacer>
				<span>boxxi</span>
				<v-spacer></v-spacer>
				<v-icon>{{ stats.wifi.rssi | wifiIcon(stats.wifi.secure) }}</v-icon>
				<!--
				<v-icon>mdi-signal-cellular-outline</v-icon>
				<v-icon>mdi-battery</v-icon>
				<span>12:30</span>
				-->
			</v-system-bar>

<!--
			<v-app-bar _bottom dense app color="orange lighten-1" dark _short>
				<v-app-bar-nav-icon @click="drawer = !drawer">
					<v-icon>$device</v-icon>
				</v-app-bar-nav-icon>

				<v-toolbar-title>paperdash.io</v-toolbar-title>

				<v-spacer></v-spacer>

				<template v-for="(link, i) in items">
					<v-btn icon :to="link.to" :key="i">
						<v-icon v-text="link.icon" />
					</v-btn>
				</template>

			</v-app-bar>
			-->

<!--
			<v-bottom-navigation
				v-model="bottomNav"
				dark
				shift
				app
			>
				<v-btn>
				<span>Device</span>
				<v-icon>$device</v-icon>
				</v-btn>

				<v-btn>
				<span>Settings</span>
				<v-icon>$settings</v-icon>
				</v-btn>
			</v-bottom-navigation>
			-->

			<v-content>
				<v-container fluid fill-height class="align-start">
					<router-view></router-view>
				</v-container>
			</v-content>
		</template>
    </v-app>
</template>

<script>
	import apiDevice from './api/device'
	import "@/assets/app.css"

    export default {
        name: 'App',
        data: () => ({
			isLoading: true,

			bottomNav: 3,

            items: [
                {title: 'Dashboard', icon: '$dashboard', to: '/'},
                //{title: 'Sandbox', icon: '$sandbox', to: '/sandbox'},
                {title: 'Wifi', icon: '$wifi', to: '/wifi'},
                {title: 'Settings', icon: '$settings', to: '/settings'},
			],
		}),
		created () {
			this.$vuetify.icons.values.device = {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/cast/baseline.svg')}
			this.$vuetify.icons.values.dashboard = {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/dashboard/baseline.svg')}
			this.$vuetify.icons.values.settings = {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/settings/baseline.svg')}
			this.$vuetify.icons.values.wifi = {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/wifi/baseline.svg')}
			this.$vuetify.icons.values.sandbox = {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/gesture/baseline.svg')}	// gesture, brush, palette,


			this.autoReloadStats();
		},
		watch: {
			stats () {
				this.isLoading = false
			}
		},
		computed: {
			stats () {
				return this.$root._data.stats;
			},
		},
		methods: {
			autoReloadStats () {
				apiDevice.getStats(stats => {
					this.$root._data.stats = stats

					setTimeout(() => {
						this.autoReloadStats()
					}, (stats.playlist.remaining + 2) * 1000)
				})
			}
		}
    };
</script>

<style scoped>
</style>