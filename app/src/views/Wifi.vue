<template>
	<v-layout fluid fill-height>
        <template v-if="isLoading">
            <v-overlay :absolute="true" :value="true">
                <v-progress-circular indeterminate size="64"></v-progress-circular>
            </v-overlay>
        </template>

		<template v-if="true">
			<v-container>

				<v-snackbar
					v-model="isSnackbar"
					:timeout="3000"
					color="success"
				>
					i8n:saved
				</v-snackbar>

				<!-- status current wifi -->
				<v-card
					max-width="344"
					class="mx-auto"
					_color="grey lighten-4"
					_flat _height="200px"
					>
					<v-template v-if="!wifiStats.connected">
						<v-card-text>
							<v-icon>$signalWifi0</v-icon>
							not connected

							<v-btn color="primary" depressed small>
								i8n:scan
							</v-btn>
						</v-card-text>
					</v-template>
					<v-template v-else>
						<v-toolbar flat>
							<v-toolbar-title class="title font-weight-light">
								<v-icon left>$signalWifi3Lock</v-icon>
								xd-design.info
							</v-toolbar-title>

							<v-spacer></v-spacer>

							<v-menu offset-y>
								<template v-slot:activator="{ on }">
									<v-btn icon small v-on="on">
										<v-icon>mdi-dots-vertical</v-icon>
									</v-btn>
								</template>
								<v-list>
									<v-list-item @click="scan()">
										<v-list-item-title>i8n:scan</v-list-item-title>
									</v-list-item>
								</v-list>
							</v-menu>
						</v-toolbar>

						<v-divider class="mx-4"></v-divider>

						<v-list dense>
							<v-list-item
								v-for="(value, key) in wifiStats"
								:key="key"
							>
								<v-list-item-title>{{ key }}</v-list-item-title>

								<v-list-item-subtitle class="text-right">
									{{ value }}
								</v-list-item-subtitle>
							</v-list-item>
						</v-list>
					</v-template>
				</v-card>


				<v-card
					style="display:none"
					max-width="344"
					class="mx-auto"
				>
					<v-card-title>
						<v-icon
							large
							left
						>
							$signalWifi3Lock
						</v-icon>
						<span class="title font-weight-light">xd-design.info</span>
						</v-card-title>

					<v-divider></v-divider>
						<v-card-text>
						<div>connected to</div>
						<p class="display-1 text--primary">
							xd-design.info
						</p>
						</v-card-text>

					<v-divider></v-divider>
					<v-list-item>
						<v-list-item-avatar color="grey_">
							<v-icon size="25">$signalWifi3Lock</v-icon>
						</v-list-item-avatar>
						<v-list-item-content>
							<v-list-item-title class="headline">xd-design.info</v-list-item-title>
							<v-list-item-subtitle>connected to</v-list-item-subtitle>
						</v-list-item-content>
					</v-list-item>

					<v-divider class="mx-4"></v-divider>

					<v-list dense>
						<v-list-item
							v-for="(value, key) in wifiStats"
							:key="key"
						>
							<v-list-item-title>{{ key }}</v-list-item-title>

							<v-list-item-subtitle class="text-right">
								{{ value }}
							</v-list-item-subtitle>
						</v-list-item>
					</v-list>
				</v-card>

<br/><br/>

				<!-- connect to wifi -->

				<v-dialog v-model="wifiConnectModal" max-width="400">
					<v-card>
						<v-card-title class="headline">
							{{ wifiConnectSSID }}
						</v-card-title>

						<v-card-text>
							<v-text-field
								v-model="wifiConnectPassword"
								:append-icon="show1 ? 'mdi-eye' : 'mdi-eye-off'"
								:type="show1 ? 'text' : 'password'"
								label="i8n:Password"
								@click:append="show1 = !show1"
							></v-text-field>
						</v-card-text>

						<v-card-actions>
							<v-spacer></v-spacer>
							<v-btn text @click="wifiConnectModal = false">i8n:Cancel</v-btn>
							<v-btn
								depressed
								:loading="isConnecting"
								color="primary darken-1"
								@click="onWifiConnect()"
							>
								i8n:Connect
							</v-btn>
						</v-card-actions>
					</v-card>
				</v-dialog>

				<v-card
					class="mx-auto"
					max-width="344"
					tile
				>
					<v-list >
						<v-subheader>Wifi found</v-subheader>
						<v-list-item-group v-model="wifiConnectSSID" color="primary">
							<v-list-item
								v-for="(wifi, i) in wifiAvailable"
								:key="i"
								:value="wifi.ssid"
								@click.stop="wifiConnectModal = true"
							>
								<v-list-item-icon>
									<v-icon v-text="getWifiIcon(wifi)"></v-icon>
								</v-list-item-icon>
								<v-list-item-content>
									<v-list-item-title v-text="wifi.ssid"></v-list-item-title>
									<v-list-item-subtitle v-text="wifi.bssid"></v-list-item-subtitle>
								</v-list-item-content>
								<v-list-item-avatar>
									<v-avatar color="teal" size="24">
										<span class="white--text headline caption">{{ wifi.channel }}</span>
									</v-avatar>
								</v-list-item-avatar>
							</v-list-item>
						</v-list-item-group>
					</v-list>
				</v-card>

			</v-container>

        </template>
    </v-layout>
</template>

<script>
	import apiDevice from '../api/device'

    export default {
		name: "Settings",
		data: () => ({
			isLoading: true,
			isSnackbar: false,
			isConnecting: false,

			wifiStats: {
				connected: true,
				ip: 'xxx.xxx.xxx.xxx',
				mac: 'xxxx-xxxx-xxxx-xxxx',
				channel: 11,
				dns: 'xxx.xxx.xxx.xxx',
				gateway: 'xxx.xxx.xxx.xxx',
			},

			wifiAvailable: [],
			wifiConnectModal: false,
			wifiConnectSSID: null,
			wifiConnectPassword: null,
			show1: false,
		}),
		created () {
			this.$vuetify.icons.values.signalWifi0 = {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_0_bar/baseline.svg')}
			this.$vuetify.icons.values.signalWifi1 = {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_1_bar/baseline.svg')}
			this.$vuetify.icons.values.signalWifi1Lock = {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_1_bar_lock/baseline.svg')}
			this.$vuetify.icons.values.signalWifi2 = {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_2_bar/baseline.svg')}
			this.$vuetify.icons.values.signalWifi2Lock = {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_2_bar_lock/baseline.svg')}
			this.$vuetify.icons.values.signalWifi3 = {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_3_bar/baseline.svg')}
			this.$vuetify.icons.values.signalWifi3Lock = {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_3_bar_lock/baseline.svg')}
			this.$vuetify.icons.values.signalWifi4 = {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_4_bar/baseline.svg')}
			this.$vuetify.icons.values.signalWifi4Lock = {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/signal_wifi_4_bar_lock/baseline.svg')}

            apiDevice.wifiScan(list => {
                this.wifiAvailable = list

                this.isLoading = false
            })
		},
		computed: {
		},
		methods: {
			getWifiIcon(wifi) {
				let icon = '$signalWifi'

				// strength
				if (wifi.rssi >= -67) {
					icon += 4
				}
				else if (wifi.rssi >= -70) {
					icon += 3
				}
				else if (wifi.rssi >= -80) {
					icon += 2
				}
				else if (wifi.rssi >= -90) {
					icon += 1
				}
				else {
					icon += 0
				}

				// secure
				if (wifi.secure !== 0 && wifi.rssi >= -90) {
					icon += 'Lock'
				}

				return icon
			},

			onWifiConnect() {
				this.isConnecting = true

				apiDevice.wifiConnect(this.wifiConnectSSID, this.wifiConnectPassword)
			}
		}
    }
</script>

<style scoped>

</style>