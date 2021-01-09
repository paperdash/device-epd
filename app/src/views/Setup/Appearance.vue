<template>
  <setup-panel
    back
    @back="stepBack"
  >
    <template #icon>
      $monochrome_photos
    </template>
    <template #headline>
      Appearance
    </template>

    <v-sheet
      width="60%"
      class="mx-auto"
    >
      <device-simulator
        :theme="form.theme"
        face="weather"
      />
    </v-sheet>

    <v-divider class="mt-5" />

    <div class="d-flex justify-center">
      <v-radio-group
        v-model="form.theme"
        row
      >
        <v-radio
          label="Light"
          value="white"
        />
        <v-radio
          label="Dark"
          value="black"
        />
      </v-radio-group>
    </div>

    <template #actions>
      <v-btn
        depressed
        block
        color="primary"
        @click="commitStep()"
      >
        Continue
      </v-btn>
    </template>
  </setup-panel>
</template>

<script>
  import { mapState, mapMutations, mapActions } from 'vuex'
  import SetupPanel from '@/components/Setup/BasePanel'
  import DeviceSimulator from '@/components/DeviceSimulator'

  export default {
    components: { DeviceSimulator, SetupPanel },
    data: () => ({
      isProcessing: false,
      form: {
        theme: '',
      },
    }),
    computed: {
      ...mapState([
        'settings',
      ]),
    },
    created () {
      this.resetChanges()
    },
    methods: {
      ...mapMutations(['updateSettings']),
      ...mapActions(['saveSettings']),

      commitStep () {
        this.isProcessing = true

        this.updateSettings({
          device: {
            theme: this.form.theme,
          },
        })

        this.saveSettings().then(() => {
          this.nextStep()
        })
      },
      resetChanges () {
        this.form.theme = this.settings.device.theme || 'black'
      },
      nextStep () {
        this.$router.push('/setup/done')
      },
      stepBack () {
        this.$router.push('/setup/name')
      },
    },
  }
</script>
