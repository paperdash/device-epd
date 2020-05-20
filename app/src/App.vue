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

			<v-system-bar app dark color="primary" _window _lights-out>
				<span>paperdash.io</span>
				<v-spacer></v-spacer>
				<span>{{ settings.device.name }}</span>
				<v-spacer></v-spacer>
				<template v-if="stats.wifi.connected">
					<v-icon>{{ stats.wifi.rssi | wifiIcon(0) }}</v-icon>
					<span>{{ stats.device.time | moment("LT") }}</span>
				</template>
				<template v-else>
					<v-btn to="/setup/wifi" icon>
						<v-icon color="red">$signalWifiOff</v-icon>
					</v-btn>
				</template>
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
					<transition-page>
						<router-view></router-view>
					</transition-page>
				</v-container>
			</v-content>
		</template>
    </v-app>
</template>

<script>
	import apiDevice from './api/device'
	import "@/assets/app.css"
	import transitionPage from "@/components/TransitionPage"

    export default {
		components: {
			transitionPage
		},
        data: () => ({
			isLoading: true,
			settings: null
		}),
		created () {
			apiDevice.getSettings(settings => {
				this.settings = settings;

				this.autoReloadStats();
			});
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
					// give esp some extra time befor fetch new data
					stats.playlist.remaining += 2

					// reset old so reactive watcher can detect a change
					if (this.$root._data.stats) {
						this.$root._data.stats.playlist.remaining = 0
					}
					this.$root._data.stats = stats

					setTimeout(() => {
						this.autoReloadStats()
					}, stats.playlist.remaining * 1000)
				})
			}
		}
    };
</script>

<style scoped>
</style>