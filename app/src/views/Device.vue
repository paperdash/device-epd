<template>
  <div class="pa-5">
    <v-card
      flat
      width="400"
      class="mx-auto"
    >
      <v-card-title class="display-2 mb-12 justify-center text-center">
        Device settings
      </v-card-title>

      <v-card-text>
        <v-text-field
          v-model="form.name"
          label="Name"
          prepend-icon="$sentiment_satisfied_alt"
        />

        <v-select
          v-model="form.theme"
          :items="optionsTheme"
          label="Appearance"
          prepend-icon="$palette"
        />

        <v-select
          disabled
          :items="getAvailableCountries"
          item-text="name"
          label="Region"
          prepend-icon="$language"
        />

        <v-select
          disabled
          :items="getAvailableLanguages"
          item-text="name"
          label="Language"
          prepend-icon="$translate"
        />

        <v-select
          disabled
          :items="getAvailableTimezones"
          label="Timezone"
          prepend-icon="$access_time"
        />
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

    device settings<br>
    - orientation<br>
    - theme<br>
    - auflösung<br>
    - system language<br>
    - system time<br>
  </div>
</template>

<script>
  import { mapState, mapGetters, mapMutations, mapActions } from 'vuex'

  export default {
    data: () => ({
      isProcessing: false,
      form: {
        name: '',
        theme: '',
      },

      // 0 thru 3 corresponding to 4 cardinal rotations
      optionsOrientation: [
        { text: 'Nord', value: 0 },
        { text: 'East', value: 1 },
        { text: 'South', value: 2 },
        { text: 'West', value: 3 },
      ],
      optionsTheme: [
        { text: 'Black', value: 'black' },
        { text: 'White', value: 'white' },
      ],
      deviceMode: [
        { text: 'Active', value: 'active' },
        { text: 'Passive', value: 'passive' },
      ],
    }),
    computed: {
      ...mapState([
        'stats',
        'settings',
      ]),
      ...mapGetters([
        'getAvailableCountries',
        'getAvailableLanguages',
        'getAvailableTimezones',
      ]),
    },
    created () {
      this.form.name = this.settings.device.name
      this.form.theme = this.settings.device.theme
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
