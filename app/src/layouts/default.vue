<template>
  <div class="d-flex flex-grow-1">
    <notifications />

    <v-navigation-drawer
      v-model="drawer"
      class="grey darken-3"
      dark
      app
      mobile-breakpoint="sm"
      _mini-variant-width="150"
      _mini-variant
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
      <v-sheet
        v-if="0"
        rounded="lg"
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
      </v-sheet>

      <v-list nav>
        <v-list-item
          v-for="page in pages"
          :key="page.label"
          :to="page.to"
          link
        >
          <v-list-item-content
            v-if="1"
            class="text-center"
          >
            <v-icon>{{ page.icon }}</v-icon>
            <v-list-item-title>{{ page.label }}</v-list-item-title>
          </v-list-item-content>

          <template v-if="0">
            <v-list-item-icon>
              <v-icon>{{ page.icon }}</v-icon>
            </v-list-item-icon>

            <v-list-item-content>
              <v-list-item-title>{{ page.label }}</v-list-item-title>
            </v-list-item-content>
          </template>
        </v-list-item>

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

      <v-progress-circular
        :rotate="-90"
        :value="40"
        class="mx-3 hidden-xs-only"
        size="40"
      >
        <v-icon>$playlist</v-icon>
      </v-progress-circular>

      <v-progress-circular
        :rotate="-90"
        :value="40"
        class="mx-3 hidden-xs-only"
        size="40"
      >
        <v-icon>$storage</v-icon>
      </v-progress-circular>

      <template v-if="stats.wifi.connected">
        <v-btn
          icon
          class="hidden-xs-only"
        >
          <v-icon>{{ stats.wifi.rssi | wifiIcon(0) }}</v-icon>
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
        <v-sheet
          min-height="70vh"
          rounded="lg"
        >
          <router-view />
        </v-sheet>
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
          icon: '$lock',
          to: '/',
        },
        {
          label: 'Device',
          icon: '$device',
          to: '/device',
        },
        {
          label: 'Playlist',
          icon: '$playlist',
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
  }
</script>

<style scoped>

</style>
