const generateKey = () => {
    const chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
    const length = chars.length;

    let random = '';
    for (let i = 0; i < length; i++) {
        random += chars.charAt(Math.floor(Math.random() * length));
    }

    return random;
}

module.exports = generateKey;