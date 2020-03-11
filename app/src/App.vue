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
				<v-img class="device-screen-image" :aspect-ratio="16/9" src="/fs/screen.bmp">
					<v-row align="end" class="lightbox white--text pa-2 fill-height">
						<v-col>
							<!--<div class="subheading">update in 2min</div>-->
							<!--<div class="body-1">heyfromjonathan@gmail.com</div>-->
						</v-col>
					</v-row>
				</v-img>

				<v-list-item>
					<v-list-item-content>
						<v-list-item-title class="title">
							paperdash.io
						</v-list-item-title>
						<v-list-item-subtitle>
							Weather
						</v-list-item-subtitle>
					</v-list-item-content>
				</v-list-item>

				<div class="d-flex justify-center py-5">
					<v-progress-circular
						class=""
						:rotate="-90"
						:size="100"
						:width="15"
						value="40"
						color="teal"
						>
						40
					</v-progress-circular>
				</div>

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
						<v-progress-linear value="74" height="20">
							19%
						</v-progress-linear>
					</v-list-item-content>
				</v-list-item>


				<!--
				<v-list>
					<v-list-item
						v-for="(link, i) in items"
						:key="i"
						:to="link.to"
						active-class="primary white--text"
						>
						<v-list-item-action>
							<v-icon v-text="link.icon" />
						</v-list-item-action>

						<v-list-item-title v-text="link.title" />
					</v-list-item>
				</v-list>
				-->
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
				<!-- -->
				<v-app-bar-nav-icon @click="drawer = !drawer">
					<v-icon>$device</v-icon>
				</v-app-bar-nav-icon>

				<v-toolbar-title>paperdash.io</v-toolbar-title>

				<v-spacer></v-spacer>

				<v-template
					v-for="(link, i) in items"
					:key="i"
					>
					<v-btn icon :to="link.to">
						<v-icon v-text="link.icon" />
					</v-btn>
				</v-template>

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

	import "@/assets/app.css"
	// eslint-disable-next-line
	//import imageBmp from "@/assets/black.bmp"
	// eslint-disable-next-line
	//import imageJpg from "@/assets/black.jpg"

    export default {
        name: 'App',
        data: () => ({
			isLoading: false,
            drawer: true,
            items: [
                {title: 'Dashboard', icon: '$dashboard', to: '/'},
                {title: 'Sandbox', icon: '$sandbox', to: '/sandbox'},
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
		},
    };
</script>

<style scoped>
  .lightbox {
    box-shadow: 0 0 20px inset rgba(0, 0, 0, 0.2);
    background-image: linear-gradient(to top, rgba(0, 0, 0, 0.4) 0%, transparent 72px);
  }
</style>