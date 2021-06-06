<template>
  <v-card
    flat
    rounded="lg"
  >
    <v-card-title class="text-h3 mb-12 justify-center text-center">
      System info
    </v-card-title>

    <v-list-item>
      <v-list-item-icon class="mr-3">
        <v-icon>$info</v-icon>
      </v-list-item-icon>
      <v-list-item-content>
        <v-list-item-title>Software</v-list-item-title>
      </v-list-item-content>
      <v-list-item-action>
        <update-dialog
          :current-firmware="new Date(stats.firmware.created *1000)"
          :current-app="new Date(stats.app.created *1000)"
        >
          <template #activator="{ on }">
            <v-btn
              outlined
              color="primary"
              v-on="on"
            >
              Update
            </v-btn>
          </template>
        </update-dialog>
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
          :value="getFSUsage"
          class="text-caption"
        >
          {{ getFSUsage }}
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
          :value="getMemoryUsage"
          class="text-caption"
        >
          {{ getMemoryUsage }}
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
          {{ memory.total | prettyBytes }}
        </v-list-item-subtitle>
      </v-list-item>

      <v-list-item>
        <v-list-item-title>Free</v-list-item-title>
        <v-list-item-subtitle class="text-right">
          {{ memory.free | prettyBytes }}
        </v-list-item-subtitle>
      </v-list-item>
    </v-list>

    <template v-if="specs">
      <v-list-item class="mt-10">
        <v-list-item-icon class="mr-3">
          <v-icon>$cable</v-icon>
        </v-list-item-icon>
        <v-list-item-content>
          <v-list-item-title>Hardware</v-list-item-title>
        </v-list-item-content>
      </v-list-item>

      <v-divider class="mx-4" />
      <v-list
        class="pb-0"
      >
        <v-list-item>
          <v-list-item-title>CPU</v-list-item-title>
          <v-list-item-subtitle class="text-right">
            {{ specs.cpu }} Mhz
          </v-list-item-subtitle>
        </v-list-item>

        <v-list-item>
          <v-list-item-title>Flash</v-list-item-title>
          <v-list-item-subtitle class="text-right">
            {{ specs.flash | prettyBytes }}
          </v-list-item-subtitle>
        </v-list-item>
      </v-list>
    </template>

    <v-divider class="mt-12" />
    <v-card-actions>
      <v-btn
        depressed
        disabled
      >
        factory reset
      </v-btn>
    </v-card-actions>
  </v-card>
</template>

<script>
  import { mapState, mapGetters } from 'vuex'
  import UpdateDialog from '@/components/UpdateDialog'

  export default {
    components: {
      UpdateDialog,
    },
    data: () => ({
    }),
    computed: {
      ...mapState(['stats']),
      ...mapGetters(['getFSUsage', 'getMemoryUsage']),
      fs () {
        return this.stats.device.fs
      },
      memoryUsage () {
        return Math.round(
          (100 / this.stats.device.heap.total) * this.stats.device.heap.free,
        )
      },
      memory () {
        return this.stats.device.heap
      },
      specs () {
        return this.stats.device?.specs
      },
    },
  }
</script>

<style scoped>
</style>
