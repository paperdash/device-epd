<template>
  <div class="d-flex flex-grow-1">
    <v-navigation-drawer
      v-model="drawer"
      class="grey darken-3"
      dark
      app
      _mini-variant-width="150"
      _mini-variant
      :clipped="clipped"
    >
      <v-sheet rounded="lg">
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
          <v-list-item-content class="text-center">
            <v-icon>{{ page.icon }}</v-icon>
            <v-list-item-title>{{ page.label }}</v-list-item-title>
          </v-list-item-content>
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

      <v-toolbar-title>paperdash</v-toolbar-title>
      <v-app-bar-nav-icon
        @click.stop="drawer = !drawer"
      />
      <v-spacer />
      <div>box name</div>
      <v-spacer />

      <v-progress-circular
        :rotate="-90"
        :value="40"
        class="mx-3"
        size="40"
      >
        <v-icon>$playlist</v-icon>
      </v-progress-circular>

      <v-progress-circular
        :rotate="-90"
        :value="40"
        class="mx-3"
        size="40"
      >
        <v-icon>$storage</v-icon>
      </v-progress-circular>

      <template v-if="stats.wifi.connected">
        <v-btn icon>
          <v-icon>{{ stats.wifi.rssi | wifiIcon(0) }}</v-icon>
        </v-btn>
        <span>{{ new Date(stats.device.time * 1000).toLocaleString() }}</span>
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

    <!--
      <v-responsive max-width="260">
        <v-text-field
          dense
          flat
          hide-details
          rounded
          solo-inverted
        />
      </v-responsive>
      -->
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

  export default {
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
          label: 'System',
          icon: '$settings',
          to: '/system',
        },
      ],
    }),
    computed: {
      ...mapState(['stats']),
    },
  }
</script>

<style scoped>

</style>
