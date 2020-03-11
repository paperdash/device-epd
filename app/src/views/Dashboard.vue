<template>
    <v-container
        fluid
        grid-list-md
        pa-2
    >
		<v-card
			class="mx-auto my-12"
			max-width="400"
			elevation="24"
			>
			<v-img class="device-screen-image" :aspect-ratio="16/9" src="/fs/screen.bmp"></v-img>
		</v-card>

    </v-container>
</template>

<script>
    //import api from '../api/core'

    export default {
		name: "Dashboard",
    }
</script>

<style scoped>

</style>