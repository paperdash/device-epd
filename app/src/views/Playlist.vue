<template>
  <div class="pa-5">
    <v-card
      flat
      width="400"
      class="mx-auto"
    >
      <v-card-title class="display-2 mb-12 justify-center text-center">
        Playlist settings
      </v-card-title>

      <v-card-text class="pb-0">
        <v-row>
          <v-col
            cols="3"
            class="text-center mt-2 pb-0"
          >
            <v-icon>$wb_sunny</v-icon>
            <br>Forecast
          </v-col>
          <v-col
            cols="6"
            class="text-center pb-0"
          >
            <v-text-field
              v-model="settings.playlist.timer"
              label="Switch every"
              type="number"
              dense
              rounded
              filled
              suffix="seconds"
              class="text--right"
            />
          </v-col>
          <v-col
            cols="3"
            class="text-center mt-2 pb-0"
          >
            <v-icon>$calendar_today</v-icon>
            <br>Calendar
          </v-col>
        </v-row>

        <ul
          v-if="0"
          class="mt-5"
        >
          <li>calendar</li>
          <li>weather forecast</li>
          <li>unsplash.com</li>
        </ul>
      </v-card-text>

      <v-divider class="mt-12" />
      <v-card-actions>
        <v-btn text>
          Cancel
        </v-btn>
        <v-spacer />
        <v-btn
          color="primary"
          text
          :loading="isProcessing"
          @click="commitChanges"
        >
          Submit
        </v-btn>
      </v-card-actions>
    </v-card>

    playlist settings<br>
    - switch every x seconds<br>
  </div>
</template>

<script>
  import { mapActions, mapGetters, mapMutations, mapState } from 'vuex'

  export default {
    components: {
    },
    data: () => ({
      isLoading: false,
    }),
    computed: {
      ...mapState([
        'stats',
        'settings',
      ]),
      ...mapGetters([
      ]),
    },
    methods: {
      ...mapMutations(['updateSettings']),
      ...mapActions(['saveSettings']),
      commitChanges () {
        this.isProcessing = true

        this.updateSettings({
          device: {
            name: this.form.name,
            theme: this.form.theme,
          },
        })

        this.saveSettings().then(() => {
          this.isProcessing = false
        })
      },
    },
  }
</script>

<style scoped>
</style>
