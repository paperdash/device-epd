<template>
  <setup-panel
    back
    @back="stepBack"
  >
    <template #icon />
    <template #headline>
      Appearance
    </template>

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
  import SetupPanel from '@/components/SetupPanel'

  export default {
    components: { SetupPanel },
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

        /*
        this.saveSettings().then(() => {
          this.nextStep()
        })
        */

        this.nextStep()
      },
      resetChanges () {
        this.form.theme = this.settings.device.theme
      },
      nextStep () {
        this.$router.push('/setup/done')
      },
      stepBack () {
        this.$router.push('/setup/weather')
      },
    },
  }
</script>
