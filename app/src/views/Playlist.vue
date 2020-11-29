<template>
  <div class="pa-5">
    <v-card
      flat
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
              v-model="form.timer"
              :disabled="!isSettingSupported('playlist.timer')"
              label="Switch every"
              :rules="[rules.minValue]"
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
      </v-card-text>

      <v-divider class="mt-12 mx-3" />

      <v-card-subtitle>
        Service
      </v-card-subtitle>
      <v-card-text>
        <v-combobox
          v-model="form.images"
          :disabled="!isSettingSupported('playlist.images')"
          :items="['https://api.paperdash.io/image/']"
          label="Image provider"
          prepend-icon="$link"
          append-outer-icon="$help"
        />
      </v-card-text>

      <v-divider class="mt-12" />
      <v-card-actions>
        <v-btn
          text
          @click="resetChanges"
        >
          Restore
        </v-btn>
        <v-spacer />
        <v-btn
          :loading="isProcessing"
          depressed
          @click="commitChanges"
        >
          <v-icon left>
            $done
          </v-icon>
          Save
        </v-btn>
      </v-card-actions>
    </v-card>

    - rename playlist to faces?
  </div>
</template>

<script>
  import { mapActions, mapGetters, mapMutations, mapState } from 'vuex'

  export default {
    components: {
    },
    data: () => ({
      isProcessing: false,
      form: {
        timer: '',
        imageProvider: '',
      },
      rules: {
        required: value => !!value || 'Required.',
        minValue: value => Number(value) >= 10 || 'Min 10 seconds',
        email: value => {
          const pattern = /^(([^<>()[\]\\.,;:\s@"]+(\.[^<>()[\]\\.,;:\s@"]+)*)|(".+"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/
          return pattern.test(value) || 'Invalid e-mail.'
        },
      },
    }),
    computed: {
      ...mapState([
        'stats',
        'settings',
      ]),
      ...mapGetters([
        'isSettingSupported',
      ]),
    },
    created () {
      this.resetChanges()
    },
    methods: {
      ...mapMutations(['updateSettings']),
      ...mapActions(['saveSettings']),
      commitChanges () {
        this.isProcessing = true

        this.updateSettings({
          playlist: {
            timer: Number(this.form.timer),
            images: this.form.images,
          },
        })

        this.saveSettings().then(() => {
          this.isProcessing = false
        })
      },
      resetChanges () {
        this.form.timer = this.settings.playlist.timer
        this.form.images = this.settings.playlist.images
      },
    },
  }
</script>

<style scoped>
</style>
