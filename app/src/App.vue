<template>
    <v-app style="_background: #e2e2e2">
		<template v-if="isLoading">
            <v-overlay :absolute="true" :value="true">
                <v-progress-circular indeterminate size="64"></v-progress-circular>
            </v-overlay>
        </template>

		<template v-else>

			<v-navigation-drawer
				v-model="drawer"
				app
			>
				<!-- https://cdn.vuetifyjs.com/images/parallax/material.jpg -->
				<v-img class="device-screen-image" :aspect-ratio="16/9" :src="device_screen_src">
					<v-row align="end" class="lightbox white--text pa-2 fill-height">
						<!--
						<v-col>
							<div class="subheading">update in 2min</div>
							<div class="body-1">heyfromjonathan@gmail.com</div>
						</v-col>
						-->
					</v-row>
				</v-img>

				<v-list-item>
					<v-list-item-icon class="mr-3">
						<v-progress-circular
							:rotate="-90"
							:size="50"
							:width="5"
							:value="playlistProgress"
							>
							{{ playlistRemaining }}
						</v-progress-circular>
					</v-list-item-icon>
					<v-list-item-content>
						<v-list-item-title class="title">
							{{ stats.playlist.current }}
						</v-list-item-title>
						<v-list-item-subtitle>
							8. March 2020
						</v-list-item-subtitle>
					</v-list-item-content>
				</v-list-item>

				<v-divider></v-divider>

				<v-list-item>
					<v-list-item-content>
						<v-list-item-title class="title">
							Weather
						</v-list-item-title>
						<v-list-item-subtitle>
							6° Salzburg
						</v-list-item-subtitle>
					</v-list-item-content>
				</v-list-item>

				<v-divider></v-divider>

				<v-list-item>
					<v-list-item-content>
						<v-list-item-title class="title">
							Calendar
						</v-list-item-title>
						<v-list-item-subtitle>
							8. March 2020
						</v-list-item-subtitle>
					</v-list-item-content>
				</v-list-item>

				<v-divider></v-divider>

				<v-list-item>
					<v-list-item-content>
						Total: {{ stats.device.fs.total | prettyBytes }}<br/>
						Free: {{ stats.device.fs.free | prettyBytes }}

						<v-progress-linear :value="fsUsagePercent" height="25">
							<template v-slot="{ value }">
								<strong class="white--text">{{ Math.ceil(value) }}%</strong>
							</template>
						</v-progress-linear>
					</v-list-item-content>
				</v-list-item>
			</v-navigation-drawer>

			<!--
			<v-system-bar app dark color="primary">
				<v-spacer></v-spacer>
				<v-icon>mdi-wifi-strength-4</v-icon>
				<v-icon>mdi-signal-cellular-outline</v-icon>
				<v-icon>mdi-battery</v-icon>
				<span>12:30</span>
			</v-system-bar>
			-->

			<v-app-bar app color="orange lighten-1" dark short>
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
	// eslint-disable-next-line
	//import imageBmp from "@/assets/black.bmp"
	// eslint-disable-next-line
	//import imageJpg from "@/assets/black.jpg"

    export default {
        name: 'App',
        data: () => ({
			isLoading: true,

			stats: null,

            drawer: true,
            items: [
                {title: 'Dashboard', icon: '$dashboard', to: '/'},
                //{title: 'Sandbox', icon: '$sandbox', to: '/sandbox'},
                {title: 'Wifi', icon: '$wifi', to: '/wifi'},
                {title: 'Settings', icon: '$settings', to: '/settings'},
			],

			playlistRemaining: 0,
			playlistTimerProgress: 30,
			device_screen_src: null
		}),
		created () {
			this.$vuetify.icons.values.device = {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/cast/baseline.svg')}
			this.$vuetify.icons.values.dashboard = {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/dashboard/baseline.svg')}
			this.$vuetify.icons.values.settings = {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/settings/baseline.svg')}
			this.$vuetify.icons.values.wifi = {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/wifi/baseline.svg')}
			this.$vuetify.icons.values.sandbox = {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/gesture/baseline.svg')}	// gesture, brush, palette,

			this.reloadStats(() => {
				this.isLoading = false
			})
		},
		watch: {
			playlistRemaining: {
                handler(value) {
                    if (value > 0) {
                        setTimeout(() => {
                            this.playlistRemaining--
                        }, 1000)
                    } else {
						this.reloadStats()
					}
                },
			}
		},
		computed: {
			fsUsagePercent () {
				let fs = this.stats.device.fs
				return parseInt(100 / fs.total * fs.used)
			},

			playlistProgress () {
				return parseInt(100 / 60 * this.stats.playlist.remaining)
			}
		},
		methods: {
			reloadStats (cb) {
				this.device_screen_src = "/current-image?" + Date.now()

				apiDevice.getStats(stats => {
					this.stats = stats

					this.playlistRemaining = stats.playlist.remaining
					this.playlistTimerProgress = parseInt(100 / 60 * stats.playlist.remaining)

					if (cb) {
						cb()
					}
				})
			}
		}
    };
</script>

<style scoped>
  .lightbox {
    box-shadow: 0 0 20px inset rgba(0, 0, 0, 0.2);
    background-image: linear-gradient(to top, rgba(0, 0, 0, 0.4) 0%, transparent 72px);
  }
</style>