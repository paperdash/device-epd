<template>
  <setup-panel>
    <v-card-title
      class="display-2 font-weight-bold mb-12 px-0 justify-center text-center"
    >
      <span class="mr-5">Hello</span> <span class="text-decoration-underline">{{ settings.device.name }}</span>
    </v-card-title>

    <v-sheet
      width="60%"
      class="mx-auto"
    >
      <device-simulator
        :theme="settings.device.theme"
        face="weather"
      />
    </v-sheet>

    <template #actions>
      <v-btn
        outlined
        block
        color="primary"
        @click="onStart"
      >
        Let's start
      </v-btn>
    </template>
  </setup-panel>
</template>

<script>
  import { mapState } from 'vuex'
  import SetupPanel from '@/components/SetupPanel'
  import DeviceSimulator from '@/components/DeviceSimulator'

  export default {
    components: { DeviceSimulator, SetupPanel },
    computed: {
      ...mapState([
        'settings',
      ]),
    },
    methods: {
      onStart () {
        this.$router.push('/')
      },
    },
  }
</script>
