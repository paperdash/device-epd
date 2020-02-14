<template>
	<v-layout fluid fill-height>
        <template v-if="isLoading">
            <v-overlay :absolute="true" :value="true">
                <v-progress-circular indeterminate size="64"></v-progress-circular>
            </v-overlay>
        </template>

		<template v-if="true">
			<v-container>

				<v-snackbar
					v-model="isSnackbar"
					:timeout="3000"
					color="success"
				>
					i8n:saved
				</v-snackbar>

				<v-card
					class="mx-auto"
					width="400"
				>
					<v-card-text>
						<v-select
							:items="deviceMode"
							v-model="settings.device_mode"
							label="i8n:Device mode"
							placeholder=""
						></v-select>

						<v-text-field
							label="i8n:Cloud server"
							v-model="settings.cloud_server"
							placeholder="i8n:https://"
						></v-text-field>

						<v-btn class="mr-4 primary" @click="onSave">i8n:save</v-btn>
					</v-card-text>
				</v-card>

			</v-container>

        </template>
    </v-layout>
</template>

<script>
    import apiDevice from '../api/device'

    export default {
		name: "Settings",
		data: () => ({
			isLoading: true,
			isSnackbar: false,

			settings: {
				device_mode: null,
				cloud_server: null
			},

			deviceMode: [
				{text: 'Active', value: 'active'},
                {text: 'Passive', value: 'passive'}
			]
		}),
		created () {
            apiDevice.getSettings(settings => {
                this.settings = settings

                this.isLoading = false
            })
		},
		methods: {
			onSave () {
				this.isLoading = true

				apiDevice.putSettings(this.settings, (data) => {
					console.log(data)
					this.isLoading = false
					this.isSnackbar = true
				})

			}
		}
    }
</script>

<style scoped>

</style>
