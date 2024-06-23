import random
import datetime

# Array of 100 sample subtitle texts
subtitle_texts = [
    "Okay, Mir-2, we're landing right over the Grand Staircase.",
    "Houston, we have a problem.",
    "I'll be back.",
    "Here's looking at you, kid.",
    "May the Force be with you.",
    "To infinity and beyond!",
    "E.T. phone home.",
    "You can't handle the truth!",
    "Elementary, my dear Watson.",
    "Go ahead, make my day.",
    "The name's Bond. James Bond.",
    "I'm the king of the world!",
    "Carpe diem. Seize the day, boys.",
    "You talking to me?",
    "Show me the money!",
    "I'll have what she's having.",
    "I see dead people.",
    "Why so serious?",
    "I'm your father.",
    "Keep your friends close, but your enemies closer.",
    "Here's Johnny!",
    "Hasta la vista, baby.",
    "You had me at hello.",
    "You're gonna need a bigger boat.",
    "I feel the need—the need for speed.",
    "I love the smell of napalm in the morning.",
    "Yippee-ki-yay, motherf***er.",
    "Say hello to my little friend!",
    "You can't win, Darth. If you strike me down, I shall become more powerful than you can possibly imagine.",
    "The first rule of Fight Club is: You do not talk about Fight Club.",
    "Just keep swimming.",
    "Do. Or do not. There is no try.",
    "It's a trap!",
    "Roads? Where we're going, we don't need roads.",
    "Wax on, wax off.",
    "I am Groot.",
    "You shall not pass!",
    "You either die a hero, or you live long enough to see yourself become the villain.",
    "I'm the Dude, so that's what you call me.",
    "You can't sit with us.",
    "On Wednesdays, we wear pink.",
    "Why so serious?",
    "It's just a flesh wound.",
    "There's no place like home.",
    "You want the moon? Just say the word and I'll throw a lasso around it and pull it down.",
    "You had me at 'hello.'",
    "They're here.",
    "Life is like a box of chocolates. You never know what you're gonna get.",
    "I'll be back.",
    "Say hello to my little friend!",
    "You can't handle the truth!",
    "May the Force be with you.",
    "Houston, we have a problem.",
    "Keep your friends close, but your enemies closer.",
    "I love the smell of napalm in the morning.",
    "I'm gonna make him an offer he can't refuse.",
    "Here's Johnny!",
    "I see dead people.",
    "Why so serious?",
    "The name's Bond. James Bond.",
    "Hasta la vista, baby.",
    "You talking to me?",
    "What we've got here is failure to communicate.",
    "I have always depended on the kindness of strangers.",
    "Stella! Hey, Stella!",
    "Oh, no, it wasn't the airplanes. It was Beauty killed the Beast.",
    "My mama always said life was like a box of chocolates. You never know what you're gonna get.",
    "You can't handle the truth!",
    "I am your father.",
    "Carpe diem. Seize the day, boys.",
    "To infinity and beyond!",
    "E.T. phone home.",
    "You're gonna need a bigger boat.",
    "Here's looking at you, kid.",
    "May the Force be with you.",
    "You had me at 'hello.'",
    "Houston, we have a problem.",
    "There's no place like home.",
    "I'll be back.",
    "Okay, Mir-2, we're landing right over the Grand Staircase.",
    "Just keep swimming.",
    "Do. Or do not. There is no try.",
    "I'm the king of the world!",
    "It's just a flesh wound.",
    "Roads? Where we're going, we don't need roads.",
    "Yippee-ki-yay, motherf***er.",
    "The first rule of Fight Club is: You do not talk about Fight Club.",
    "Why so serious?",
    "You can't win, Darth. If you strike me down, I shall become more powerful than you can possibly imagine.",
    "Elementary, my dear Watson.",
    "I'm the Dude, so that's what you call me.",
    "You can't sit with us.",
    "On Wednesdays, we wear pink.",
    "Wax on, wax off.",
    "I'm your father.",
]

# Function to generate random subtitles
def generate_subtitles(num_subtitles):
    subtitles = []

    for i in range(num_subtitles):
        start_time = datetime.datetime(2024, 1, 1, random.randint(0, 22), random.randint(0, 59), random.randint(0, 59), random.randint(0, 999999))
        time_increment =  datetime.timedelta(seconds=random.randint(5, 30))  # Random duration between 5 to 30 seconds
        end_time = start_time + time_increment

        subtitle = {
            "id": random.randint(1, 10000),
            "start_time": start_time.strftime("%H:%M:%S,%f")[:-3],
            "end_time": end_time.strftime("%H:%M:%S,%f")[:-3],
            "text": random.choice(subtitle_texts)
        }
        subtitles.append(subtitle)
        start_time = end_time + datetime.timedelta(seconds=1)  # 1 second gap

    return subtitles

# Function to write subtitles to an SRT file
def write_srt_file(filename, subtitles):
    with open(filename, "w") as file:
        for subtitle in subtitles:
            file.write(f"{subtitle['id']}\n")
            file.write(f"{subtitle['start_time']} --> {subtitle['end_time']}\n")
            file.write(f"{subtitle['text']}\n\n")

# Generate random subtitles and write to file
num_subtitles = 5000000  # Number of subtitles to generate
subtitles = generate_subtitles(num_subtitles)
write_srt_file("generated_subtitles_5M.srt", subtitles)

print("Generated SRT file: generated_subtitles.srt")
