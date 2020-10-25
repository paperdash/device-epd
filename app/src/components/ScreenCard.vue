<template>
  <v-card
    color="grey lighten-4"
    _outlined
  >
    <v-img
      :aspect-ratio="16/9"
      :src="device_screen_src"
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

    <v-card-actions style="position: relative">
      <router-link to="/settings">
        <v-btn
          color="orange"
          dark
          absolute
          top
          right
          fab
          elevation="2"
        >
          <v-icon>$settings</v-icon>
        </v-btn>
      </router-link>

      <v-progress-circular
        :rotate="-90"
        :size="50"
        :width="5"
        :value="playlistProgress"
      >
        {{ playlistRemainingCountdown }}
      </v-progress-circular>
      <span class="headline pl-3">{{ playlistCurrent }}</span>
    </v-card-actions>
  </v-card>
</template>

<script>
  export default {
    data: () => ({
      device_screen_src: null,
      playlistRemainingCountdown: 0,
    }),
    computed: {
      playlistProgress () {
        return parseInt((100 / 60) * this.playlistRemainingCountdown)
      },
      playlistRemaining () {
        return this.$root._data.stats.playlist.remaining
      },
      playlistCurrent () {
        return this.$root._data.stats.playlist.current
      },
    },
    watch: {
      playlistRemaining (val) {
        if (val > 0) {
          this.playlistRemainingCountdown = val
          this.device_screen_src = '/current-image3?' + Date.now()
        }
      },
    },
    created () {
      this.playlistRemainingCountdown = this.playlistRemaining
      this.device_screen_src = '/current-image3?' + Date.now()

      setInterval(() => {
        if (this.playlistRemainingCountdown > 0) {
          this.playlistRemainingCountdown--
        }
      }, 1000)
    },
  }
</script>
