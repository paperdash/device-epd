<template>
  <v-card
    flat
    rounded="lg"
  >
    <v-card-title class="display-2 mb-12 justify-center text-center">
      Device settings
    </v-card-title>

    <div>
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
        v-model="form.language"
        disabled
        :items="getAvailableLanguages"
        item-text="native"
        item-value="code"
        label="Language"
        prepend-icon="$translate"
      />

      <v-autocomplete
        v-model="form.timezone"
        :items="getAvailableTimezonesSorted"
        item-value="name"
        item-text="name"
        label="Timezone"
        prepend-icon="$access_time"
        return-object
      >
        <template #item="{ item }">
          (GMT{{ item.utcOffsetStr }}) {{ item.name }}
        </template>
        <template #selection="{item}">
          (GMT{{ item.utcOffsetStr }}) {{ item.name }}
        </template>
      </v-autocomplete>
    </div>

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
</template>

<script>
  import { mapState, mapGetters, mapMutations, mapActions } from 'vuex'

  export default {
    data: () => ({
      isProcessing: false,
      form: {
        name: '',
        theme: '',
        country: '',
        timezone: '',
        language: '',
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
        'getTimezone',
      ]),
      getAvailableTimezonesSorted () {
        return this.getAvailableTimezones.slice(0).sort((a, b) => {
          //  .sort((a,b) => a.time===b.time ? 0 : (a.time < b.time ? -1 : 1));
          if (a.utcOffset === b.utcOffset) {
            return 0
          } else {
            return a.utcOffset < b.utcOffset ? -1 : 1
          }
          // return a.utcOffset < b.utcOffset ? -1 : 1
        })
      },
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
          device: {
            name: this.form.name,
            theme: this.form.theme,
          },
          system: {
            country: this.form.country,
            language: this.form.language,
            timezone: this.form.timezone.name,
            utc: this.form.timezone.utcOffset * 60,
            dst: this.form.timezone.dstOffset * 60,
          },
        })

        this.saveSettings().then(() => {
          this.isProcessing = false
        })
      },
      resetChanges () {
        this.form.name = this.settings.device.name
        this.form.theme = this.settings.device.theme
        this.form.country = this.settings.system.country
        this.form.timezone = this.getTimezone(this.settings.system.timezone)
        this.form.language = this.settings.system.language
      },
    },
  }
</script>

<style scoped>
</style>
