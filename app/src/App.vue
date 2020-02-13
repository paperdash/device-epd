<template>
    <v-app style="background: #e2e2e2">
        <v-navigation-drawer
            v-model="drawer"
			app
        >
			<v-img :aspect-ratio="16/9" src="https://cdn.vuetifyjs.com/images/parallax/material.jpg">
				<v-row align="end" class="lightbox white--text pa-2 fill-height">
				<v-col>
					<div class="subheading">update in 2min</div>
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
						display
					</v-list-item-subtitle>
				</v-list-item-content>
			</v-list-item>

			<v-divider></v-divider>

            <v-list>
                <v-list-item
                    v-for="(link, i) in items"
                    :key="i"
                    :to="link.to"
                    active-class="primary white--text"
                    >
                    <v-list-item-action>
                        <component :is=link.icon class="icon icon-white"></component>
                    </v-list-item-action>

                    <v-list-item-title v-text="link.title" />
                </v-list-item>
            </v-list>
        </v-navigation-drawer>

        <v-content>
            <v-container fluid fill-height class="align-start">
                <router-view></router-view>
            </v-container>
        </v-content>
    </v-app>
</template>

<script>
	//complete: {component: () => import(/* webpackChunkName: "icons" */'!vue-svg-loader!@material-icons/svg/svg/check/baseline.svg')}

	import iconDashboard from '!vue-svg-loader!@material-icons/svg/svg/check/baseline.svg'


    export default {
        name: 'App',
        components: {
			iconDashboard
        },
        data: () => ({
            drawer: true,
            items: [
                {title: 'Dashboard', icon: 'iconDashboard', to: '/'},
                {title: 'Settings', icon: '', to: '/settings'},
                {title: 'Sandbox', icon: '', to: '/sandbox'},
            ],
        })
    };
</script>

<style scoped>
  .lightbox {
    box-shadow: 0 0 20px inset rgba(0, 0, 0, 0.2);
    background-image: linear-gradient(to top, rgba(0, 0, 0, 0.4) 0%, transparent 72px);
  }
</style>