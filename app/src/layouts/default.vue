<template>
  <div class="d-flex flex-grow-1">
    <notifications />

    <v-navigation-drawer
      v-model="drawer"
      class="grey darken-3"
      dark
      app
      :permanent="$vuetify.breakpoint.mdOnly"
      :temporary="$vuetify.breakpoint.smAndDown"
      :clipped="clipped"
    >
      <template
        #prepend
      >
        <v-img
          :aspect-ratio="16/9"
          src="/api/device/screen"
        >
          <template #placeholder>
            <v-row
              class="fill-height ma-0 grey"
              align="center"
              justify="center"
            >
              <v-progress-circular
                indeterminate
                color="grey lighten-5"
              />
            </v-row>
          </template>
        </v-img>
      </template>
      <template #append>
        <v-list class="py-0">
          <v-divider />
          <v-list-item
            link
            to="/setup"
          >
            <v-list-item-content class="text-center">
              <v-icon>$support</v-icon>
              <v-list-item-title>Assistant</v-list-item-title>
            </v-list-item-content>
          </v-list-item>
        </v-list>
      </template>

      <v-list nav>
        <v-list-item
          v-for="page in pages"
          :key="page.label"
          :to="page.to"
          link
        >
          <v-list-item-content
            v-if="0"
            class="text-center"
          >
            <v-icon>{{ page.icon }}</v-icon>
            <v-list-item-title>{{ page.label }}</v-list-item-title>
          </v-list-item-content>

          <template v-if="1">
            <v-list-item-icon>
              <v-icon>{{ page.icon }}</v-icon>
            </v-list-item-icon>

            <v-list-item-content>
              <v-list-item-title>{{ page.label }}</v-list-item-title>
            </v-list-item-content>
          </template>
        </v-list-item>

        <!--
        <v-divider />
        <v-list-item
          link
          to="/setup"
        >
          <v-list-item-content class="text-center">
            <v-icon>$support</v-icon>
            <v-list-item-title>Assistant</v-list-item-title>
          </v-list-item-content>
        </v-list-item>
        -->
      </v-list>
    </v-navigation-drawer>

    <v-app-bar
      app
      color="orange darken-2"
      dark
      flat_
      :clipped-left="clipped"
    >
      <v-btn
        _class="hidden-xs-and-down"
        to="/"
        icon
        color="transparent"
      >
        <img
          width="48"
          height="48"
          src="@/assets/logo.png"
          alt="logo"
        >
      </v-btn>

      <v-toolbar-title class="hidden-xs-only">
        paperdash
      </v-toolbar-title>
      <v-spacer />
      <div>{{ settings.device.name }}</div>
      <v-spacer />

      <div class="hidden-xs-only mx-3">
        <v-icon>$slideshow</v-icon>
        <v-progress-linear
          :value="40"
          size="40"
          color="white"
        />
      </div>
      <div class="hidden-xs-only mx-3">
        <v-icon>$storage</v-icon>
        <v-progress-linear
          :value="72"
          color="white"
        />
      </div>

      <template v-if="stats.wifi.connected">
        <v-btn
          icon
          class="hidden-xs-only"
        >
          <v-icon>
            {{ stats.wifi.rssi | wifiIcon(0) }}
          </v-icon>
        </v-btn>
        <span class="hidden-xs-only">{{ new Date(stats.device.time * 1000).toLocaleString(undefined, {month: 'numeric', day: 'numeric', year: 'numeric', hour: '2-digit', minute: '2-digit'}) }}</span>
      </template>
      <template v-else>
        <v-btn
          to="/setup/wifi"
          icon
        >
          <v-icon color="red">
            $signalWifiOff
          </v-icon>
        </v-btn>
      </template>

      <v-app-bar-nav-icon
        class="hidden-md-and-up"
        @click.stop="drawer = !drawer"
      />
    </v-app-bar>

    <v-main class="grey lighten-3">
      <v-container>
        <v-col
          class="_md _offset-md"
          cols="12"
          sm="8"
          offset-sm="2"
          md="8"
          offset-md="2"
          lg="6"
          offset-lg="3"
        >
          <v-sheet
            _min-width="1vh"
            _max-width="50vh"
            min-height="70vh"
            rounded="lg"
          >
            <router-view />
          </v-sheet>
        </v-col>
      </v-container>
    </v-main>
  </div>
</template>

<script>
  import { mapState } from 'vuex'
  import Notifications from '@/components/Notifications'

  export default {
    components: { Notifications },
    data: () => ({
      drawer: true,
      clipped: true,
      pages: [
        {
          label: 'Dashboard',
          icon: '$dashboard',
          to: '/',
        },
        {
          label: 'Device',
          icon: '$device',
          to: '/device',
        },
        {
          label: 'Playlist',
          icon: '$slideshow',
          to: '/playlist',
        },
        {
          label: 'Weather',
          icon: '$wb_sunny',
          to: '/weather',
        },
        {
          label: 'Wifi',
          icon: '$signalWifi3',
          to: '/wifi',
        },
        {
          label: 'System',
          icon: '$settings',
          to: '/system',
        },
      ],
    }),
    computed: {
      ...mapState(['stats', 'settings']),
    },
    created () {
      this.drawer = !this.$vuetify.breakpoint.smAndDown
    },
  }
</script>

<style scoped>

</style>
