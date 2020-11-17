<template>
  <div class="pa-5">
    <v-card
      flat
      width="400"
      class="mx-auto"
    >
      <v-list-item>
        <v-list-item-icon class="mr-3">
          <v-icon>$info</v-icon>
        </v-list-item-icon>
        <v-list-item-content>
          <v-list-item-title>Software</v-list-item-title>
        </v-list-item-content>
        <v-list-item-action>
          <v-btn
            outlined
            color="primary"
          >
            Update
          </v-btn>
        </v-list-item-action>
      </v-list-item>
      <v-divider class="mx-4" />
      <v-list
        class="pb-0"
      >
        <v-list-item three-line>
          <v-list-item-content>
            <v-list-item-title>Firmware</v-list-item-title>
            <v-list-item-subtitle class="text--disabled">
              Rev: {{ stats.firmware.rev }}
            </v-list-item-subtitle>
            <v-list-item-subtitle class="text--disabled">
              Build: {{ new Date(stats.firmware.created * 1000).toLocaleString() }}
            </v-list-item-subtitle>
          </v-list-item-content>
        </v-list-item>

        <v-list-item three-line>
          <v-list-item-content>
            <v-list-item-title>App</v-list-item-title>
            <v-list-item-subtitle class="text--disabled">
              Rev: {{ stats.app.rev }}
            </v-list-item-subtitle>
            <v-list-item-subtitle class="text--disabled">
              Build: {{ new Date(stats.app.created * 1000).toLocaleString() }}
            </v-list-item-subtitle>
          </v-list-item-content>
        </v-list-item>
      </v-list>

      <v-list-item class="mt-10">
        <v-list-item-icon class="mr-3">
          <v-icon>$storage</v-icon>
        </v-list-item-icon>
        <v-list-item-content>
          <v-list-item-title>Storage</v-list-item-title>
        </v-list-item-content>
        <v-list-item-avatar>
          <v-progress-circular
            :rotate="-90"
            :value="fsUsage"
            class="caption"
          >
            {{ fsUsage }}
          </v-progress-circular>
        </v-list-item-avatar>
      </v-list-item>
      <v-divider class="mx-4" />
      <v-list
        class="pb-0"
      >
        <v-list-item>
          <v-list-item-title>Total</v-list-item-title>
          <v-list-item-subtitle class="text-right">
            {{ fs.total | prettyBytes }}
          </v-list-item-subtitle>
        </v-list-item>

        <v-list-item>
          <v-list-item-title>Free</v-list-item-title>
          <v-list-item-subtitle class="text-right">
            {{ fs.free | prettyBytes }}
          </v-list-item-subtitle>
        </v-list-item>
      </v-list>
      <v-progress-linear
        height="25"
        :value="fsUsage"
        dark
        rounded
      >
        <template #default="{ value }">
          <strong>{{ Math.ceil(value) }}%</strong>
        </template>
      </v-progress-linear>

      <v-list-item class="mt-10">
        <v-list-item-icon class="mr-3">
          <v-icon>$memory</v-icon>
        </v-list-item-icon>
        <v-list-item-content>
          <v-list-item-title>Memory</v-list-item-title>
        </v-list-item-content>
        <v-list-item-avatar>
          <v-progress-circular
            :rotate="-90"
            :value="fsUsage"
            class="caption"
          >
            {{ fsUsage }}
          </v-progress-circular>
        </v-list-item-avatar>
      </v-list-item>

      <v-divider class="mx-4" />
    </v-card>
  </div>
</template>

<script>
  import { mapState } from 'vuex'

  export default {
    components: {
    },
    data: () => ({
      isLoading: false,
    }),
    computed: {
      ...mapState(['stats']),
      fsUsage () {
        return Math.round(
          (100 / this.stats.device.fs.total) * this.stats.device.fs.used,
        )
      },
      fs () {
        return this.stats.device.fs
      },

    },
  }
</script>

<style scoped>
</style>
