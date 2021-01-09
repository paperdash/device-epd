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
      clipped
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
        <v-card
          flat
          color="transparent"
          disabled
          class="text-center"
        >
          <v-card-text>
            paperdash.io
          </v-card-text>
        </v-card>
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
          <v-list-item-icon>
            <v-icon>{{ page.icon }}</v-icon>
          </v-list-item-icon>

          <v-list-item-content>
            <v-list-item-title>{{ page.label }}</v-list-item-title>
          </v-list-item-content>
        </v-list-item>
      </v-list>
    </v-navigation-drawer>

    <v-app-bar
      app
      color="orange darken-2"
      dark
      clipped-left
    >
      <v-toolbar-title>
        {{ settings.device.name }}
      </v-toolbar-title>

      <v-spacer />

      <div class="hidden-xs-only mx-3">
        <v-icon :title="'FS usage: ' + getFSUsage + '%'">
          $storage
        </v-icon>
        <v-progress-linear
          :value="getFSUsage"
          color="white"
        />
      </div>

      <template v-if="stats.wifi.connected">
        <v-btn
          icon
          class="hidden-xs-only"
          :title="'RSSI: ' + stats.wifi.rssi"
        >
          <v-icon>
            {{ stats.wifi.rssi | wifiIcon(0) }}
          </v-icon>
        </v-btn>
        <span class="hidden-xs-only">{{ new Date(stats.device.time * 1000).toLocaleString(undefined, {month: 'numeric', day: 'numeric', year: 'numeric', hour: '2-digit', minute: '2-digit'}) }}</span>
      </template>
      <template v-else>
        <v-btn
          to="/wifi"
          icon
        >
          <v-icon>
            $signalWifiOff
          </v-icon>
        </v-btn>
      </template>

      <v-app-bar-nav-icon
        class="hidden-md-and-up"
        @click.stop="drawer = !drawer"
      />
    </v-app-bar>

    <v-main
      :class="[{'grey lighten-3': !$vuetify.breakpoint.xsOnly}]"
    >
      <v-col
        class="px-0"
        cols="12"
        sm="8"
        offset-sm="2"
        md="8"
        offset-md="2"
        lg="6"
        offset-lg="3"
      >
        <v-sheet
          min-height="70vh"
          rounded="lg"
        >
          <transition-page>
            <router-view class="pa-5" />
          </transition-page>
        </v-sheet>
      </v-col>
    </v-main>
  </div>
</template>

<script>
  import { mapState, mapGetters } from 'vuex'
  import Notifications from '@/components/Notifications'
  import TransitionPage from '@/components/TransitionPage'

  export default {
    components: { TransitionPage, Notifications },
    data: () => ({
      drawer: true,
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
          icon: '$wifi',
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
      ...mapGetters(['getFSUsage']),
    },
    created () {
      this.drawer = !this.$vuetify.breakpoint.smAndDown
    },
  }
</script>

<style scoped>

</style>
