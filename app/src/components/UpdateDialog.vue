<template>
  <v-dialog
    v-model="dialog"
    max-width="400"
    :fullscreen="$vuetify.breakpoint.mobile"
  >
    <template #activator="{ on }">
      <slot
        name="activator"
        :on="on"
      />
    </template>

    <v-card
      :disabled="updateProgress > 0"
    >
      <v-toolbar
        dark
        color="orange darken-2"
        flat
        class="mb-5"
      >
        <v-toolbar-title>
          System update
        </v-toolbar-title>
        <v-spacer />
        <v-btn
          icon
          dark
          @click="dialog = false"
        >
          <v-icon>$close</v-icon>
        </v-btn>
      </v-toolbar>

      <v-card-text>
        <v-file-input
          v-model="file"
          show-size
          accept="application/octet-stream"
          label="File"
        />
      </v-card-text>

      <v-card-text v-if="updateType">
        <v-alert
          border="left"
          icon="$info"
          outlined
          type="info"
        >
          {{ updateType }}: {{ file.lastModifiedDate.toLocaleString() }}<br>
          <small>Current: {{ currentVersion.toLocaleString() }}</small>
        </v-alert>
      </v-card-text>
      <v-card-text v-else-if="file">
        <v-alert
          border="left"
          icon="$error"
          outlined
          type="error"
        >
          Invalid file
        </v-alert>
      </v-card-text>

      <v-divider />

      <v-card-actions>
        <template v-if="!updateProgress">
          <v-btn
            text
            href="https://github.com/paperdash"
            target="_blank"
          >
            Get firmware
          </v-btn>
          <v-spacer />
          <v-btn
            :disabled="!updateType"
            outlined
            color="warning"
            @click="onSystemUpdate()"
          >
            Update system
          </v-btn>
        </template>
        <template v-else>
          <v-progress-linear
            v-model="updateProgress"
            :indeterminate="updateProgress === 100"
            height="10"
            color="orange darken-1"
            rounded
          />
        </template>
      </v-card-actions>
    </v-card>

    <v-dialog
      :value="updateResult !== null"
      persistent
      max-width="400"
    >
      <v-card class="pt-1">
        <div class="ma-5">
          <v-alert
            v-if="updateResult"
            outlined
            border="left"
            icon="$success"
            type="success"
          >
            update result...
          </v-alert>
          <v-alert
            v-if="!updateResult"
            outlined
            border="left"
            icon="$error"
            type="error"
          >
            update result...
          </v-alert>
        </div>

        <v-divider />

        <v-card-actions>
          <v-spacer />
          <v-btn
            text
            @click="onUpdateDone()"
          >
            OK
          </v-btn>
        </v-card-actions>
      </v-card>
    </v-dialog>
  </v-dialog>
</template>

<script>
  import axios from 'axios'

  export default {
    name: 'UpdateDialog',
    props: {
      currentFirmware: {
        type: Date,
        required: true,
      },
      currentApp: {
        type: Date,
        required: true,
      },
    },
    data: () => ({
      dialog: false,
      file: null,
      updateProgress: null,
      updateResult: null,
    }),
    computed: {
      updateType () {
        if (this.file) {
          switch (this.file.name) {
            case 'firmware.bin':
              return 'Firmware'
            case 'spiffs.bin':
              return 'APP'
          }
        }
        return null
      },
      currentVersion () {
        if (this.file) {
          switch (this.file.name) {
            case 'firmware.bin':
              return this.currentFirmware
            case 'spiffs.bin':
              return this.currentApp
          }
        }
        return null
      },
    },
    methods: {
      onSystemUpdate () {
        const self = this
        self.updateProgress = 0

        const config = {
          onUploadProgress: function (progressEvent) {
            self.updateProgress = Math.round(
              (progressEvent.loaded * 100) / progressEvent.total,
            )
          },
        }

        const formData = new FormData()
        formData.append('update', this.file)

        axios
          .post('/update', formData, config)
          .then(response => {
            console.log(response.data)

            self.updateProgress = null
            self.updateResult = response.data.success
          })
          .catch(response => {
            console.log(response)

            self.updateProgress = null
            self.updateResult = false
          })
      },

      onUpdateDone () {
        if (this.updateResult === true) {
          window.location = '/'
        } else {
          this.file = null
          this.updateProgress = 0
          this.updateResult = null
        }
      },
    },
  }
</script>

<style scoped>

</style>
