<template>
  <v-card outlined>
    <!--
    <v-toolbar
      extended
      extension-height="100"
      dark
      prominent
      src="https://cdn.vuetifyjs.com/images/backgrounds/vbanner.jpg"
    >
      <v-app-bar-nav-icon></v-app-bar-nav-icon>

      <v-toolbar-title>Title</v-toolbar-title>

      <v-spacer></v-spacer>

      <v-btn icon>
        <v-icon>mdi-magnify</v-icon>
      </v-btn>

      <v-btn icon>
        <v-icon>mdi-heart</v-icon>
      </v-btn>

      <v-btn icon>
        <v-icon>mdi-dots-vertical</v-icon>
      </v-btn>
    </v-toolbar>
    -->

    <!-- bug, border radius oben geht mit skeleton nicht -->
    <v-skeleton-loader type="image" tile :loading="false">
      <v-img class="device-screen-image" :aspect-ratio="16/9" :src="device_screen_src"></v-img>
    </v-skeleton-loader>
		<!--
    <v-btn absolute dark fab top right color="white">
      <v-icon>$settings</v-icon>
    </v-btn>
		-->
<!--
    <v-card-text style="height: 100px; position: relative">
        <v-btn color="pink" dark absolute top right fab>
          <v-icon>mdi-plus</v-icon>
        </v-btn>
    </v-card-text>
		-->

    <v-card-actions style="position: relative">
			<v-btn color="white" _dark absolute top right fab elevation="2">
          <v-icon>$settings</v-icon>
        </v-btn>

      <v-progress-circular
        :rotate="-90"
        :size="50"
        :width="5"
        :value="playlistProgress"
      >{{ playlistRemainingCountdown }}</v-progress-circular>
      <span class="headline pl-3">{{ playlistCurrent }}</span>
    </v-card-actions>
  </v-card>
</template>

<script>
export default {
  data: () => ({
    device_screen_src: null,
    playlistRemainingCountdown: 0
  }),
  created() {
    this.playlistRemainingCountdown = this.playlistRemaining;
    this.device_screen_src = "/current-image?" + Date.now();
  },
  computed: {
    playlistProgress() {
      return parseInt((100 / 60) * this.playlistRemainingCountdown);
    },
    playlistRemaining() {
      return this.$root._data.stats.playlist.remaining;
    },
    playlistCurrent() {
      return this.$root._data.stats.playlist.current;
    }
  },
  watch: {
    playlistRemaining(val) {
      this.playlistRemainingCountdown = val;
      this.device_screen_src = "/current-image?" + Date.now();
    },

    playlistRemainingCountdown(val) {
      if (val > 0) {
        setTimeout(() => {
          this.playlistRemainingCountdown--;
        }, 1000);
      }
    }
  }
};
</script>
