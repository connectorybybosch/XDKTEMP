import tweepy
import credenciales
CONSUMER_KEY = credenciales.CONSUMER_KEY
CONSUMER_SECRET = credenciales.CONSUMER_SECRET


ACCESS_TOKEN = credenciales.ACCESS_TOKEN
ACCESS_TOKEN_SECRET = credenciales.ACCESS_TOKEN_SECRET
auth = tweepy.OAuthHandler (CONSUMER_KEY, CONSUMER_SECRET)
auth.set_access_token (ACCESS_TOKEN, ACCESS_TOKEN_SECRET)
api = tweepy.API (auth)

def ejecutar(mensaje):
    try:
        status = api.update_status (mensaje)
    except tweepy.error.TweepError:
        pass
