#include <SDL3/SDL.h>
#include "config.h"

SDL_Colors COLORS = {
    (SDL_Color){255, 255, 255, 255}, //white
    (SDL_Color){0, 0, 0, 255},       //black
    (SDL_Color){200, 165, 0, 255},   //yellow
    (SDL_Color){255, 0, 0, 255},     //red
    (SDL_Color){10, 200, 0, 255},    //green
    (SDL_Color){30, 30, 30, 255},    //dark gray
    (SDL_Color){55, 55, 55, 255},    //gray
    (SDL_Color){120, 120, 120, 255}, //light gray
    (SDL_Color){75, 75, 75, 225}     //menu background
};

WindowBools window_bools = {
    true,
    false,
    false,
    false,
    false
};

Buttons buttons = {
    (SDL_FRect){0, 0, 0, 0},
    (SDL_FRect){0, 0, 0, 0},
    (SDL_FRect){0, 0, 0, 0},
    (SDL_FRect){0, 0, 0, 0},
    (SDL_FRect){0, 0, 0, 0},
    (SDL_FRect){0, 0, 0, 0}
};

const char *QWERTY[] = {"QWERTYUIOP","ASDFGHJKL","ZXCVBNM"};

char word_answer[] = "";
int tries = 6;
int word_length = 5;
char letter_grid[MAX_ATTEMPTS][MAX_WORD_LENGTH];
SDL_FRect grid[MAX_ATTEMPTS][MAX_WORD_LENGTH];
SDL_Color grid_colors[MAX_ATTEMPTS][MAX_WORD_LENGTH];
SDL_FRect keyboard_grid[3][10];
SDL_Color keyboard_colors[3][10];

int focused_attempt = 1;
int focused_letter_number = 1;

char error_message[40] = "Word not in word list!";
Uint32 timer_start = 0;
Uint32 timer_duration = 5000;
bool timer_running = false;

Checkbox hard_mode = {(SDL_FRect){0, 0, 20, 20}, false};

Data data = {0, 0, 0, 0};

const char *A_WORDS_5[] = {
    "aback", "abase", "abate", "abbey", "abbot", "abhor", "abide", "abled", "abode", "abort", "about", "above", "abuse", "abyss", "acorn", "acrid", "acted", "actin", "actor", "acute", "adage", "adapt", "added", "adept", "adieu", "admin", "admit", "adobe", "adopt", "adore", "adorn", "adult", "aegis", "affix", "afire", "afoot", "afoul", "after", "again", "agape", "agate", "agent", "agile", "aging", "aglow", "agony", "agora", "agree", "ahead", "aided", "aider", "aimed", "aired", "aisle", "alamo", "alarm", "album", "alert", "algae", "alias", "alibi", "alien", "align", "alike", "alive", "allay", "alley", "allot", "allow", "alloy", "aloft", "aloha", "alone", "along", "aloof", "aloud", "alpha", "altar", "alter", "amass", "amaze", "amber", "amble", "amend", "amino", "amiss", "amity", "among", "amour", "ample", "amply", "amuse", "angel", "anger", "angle", "angry", "angst", "anime", "ankle", "annex", "annoy", "annul", "anode", "antic", "anvil", "aorta", "apart", "aphid", "aping", "apnea", "apple", "apply", "apron", "aptly", "arbor", "ardor", "arena", "argon", "argue", "arias", "arise", "arjun", "armed", "armor", "aroma", "arose", "array", "arrow", "arson", "artsy", "ascot", "ashen", "aside", "asked", "askew", "aspen", "assay", "asset", "atlas", "atoll", "atone", "attic", "audio", "audit", "augur", "aunty", "avail", "avant", "avert", "avian", "avoid", "await", "awake", "award", "aware", "awash", "awful", "awoke", "axial", "axiom", "axion", "azure"
};

const char *B_WORDS_5[] = {
    "bacon", "badge", "badly", "bagel", "baggy", "baked", "baker", "baler", "balmy", "banal", "banco", "banjo", "barge", "baron", "barre", "basal", "based", "basic", "basil", "basin", "basis", "baste", "batch", "bathe", "baton", "batty", "bawdy", "bayou", "beach", "beady", "beard", "beast", "beech", "beefy", "befit", "began", "begat", "beget", "begin", "begun", "beige", "being", "belch", "belie", "bella", "belle", "belly", "below", "bench", "benny", "beret", "berry", "berth", "beset", "betel", "bevel", "bezel", "bible", "bicep", "biddy", "bigot", "biker", "bilge", "billy", "binge", "bingo", "biome", "birch", "birth", "bison", "bitch", "bitty", "black", "blade", "blame", "blanc", "bland", "blank", "blare", "blast", "blaze", "bleak", "bleat", "bleed", "bleep", "blend", "bless", "blimp", "blind", "bling", "blink", "bliss", "blitz", "bloat", "block", "bloke", "blond", "blood", "bloom", "blown", "bluer", "bluff", "blunt", "blurb", "blurt", "blush", "board", "boast", "bobby", "bogus", "boner", "boney", "bongo", "bonus", "booby", "booed", "boost", "booth", "booty", "booze", "boozy", "borax", "bored", "borne", "bosom", "bossy", "botch", "bough", "boule", "bound", "bowed", "bowel", "bowie", "boxed", "boxer", "brace", "braid", "brain", "brake", "brand", "brash", "brass", "brave", "bravo", "brawl", "brawn", "bread", "break", "breed", "brett", "briar", "bribe", "brick", "bride", "brief", "brill", "brine", "bring", "brink", "briny", "brisk", "britt", "broad", "brock", "broil", "broke", "brood", "brook", "broom", "broth", "brown", "brunt", "brush", "brute", "bucky", "buddy", "budge", "buffy", "buggy", "bugle", "build", "built", "bulge", "bulky", "bully", "bumpy", "bunch", "bundy", "bunny", "burke", "burly", "burnt", "burst", "bused", "bushy", "busty", "butch", "butte", "buxom", "buyer", "bylaw"
};

const char *C_WORDS_5[] = {
    "cabal", "cabby", "cabin", "cable", "cacao", "cache", "cacti", "caddy", "cadet", "cadre", "caged", "cagey", "cairn", "calif", "camel", "cameo", "canal", "candy", "canny", "canoe", "canon", "caper", "carat", "cared", "cargo", "carol", "carry", "carte", "carve", "caste", "catch", "cater", "catty", "caulk", "cause", "cavil", "cease", "cedar", "ceded", "cello", "chafe", "chaff", "chain", "chair", "chalk", "champ", "chant", "chaos", "chard", "charm", "chart", "chase", "chasm", "cheap", "cheat", "check", "cheek", "cheer", "chemo", "cheng", "chess", "chest", "chevy", "chick", "chide", "chief", "child", "chile", "chili", "chill", "chime", "china", "chino", "chirp", "chock", "choir", "choke", "chord", "chore", "chose", "chuck", "chump", "chunk", "churn", "chute", "cider", "cigar", "cinch", "circa", "cisco", "cited", "civic", "civil", "clack", "claim", "clamp", "clang", "clank", "clash", "clasp", "class", "clean", "clear", "cleat", "cleft", "clegg", "clerk", "click", "cliff", "climb", "cling", "clink", "clint", "cloak", "clock", "clone", "close", "cloth", "cloud", "clout", "clove", "clown", "cluck", "clued", "clump", "clung", "coach", "coast", "cobra", "cocky", "cocoa", "coded", "codex", "colin", "colon", "color", "combo", "comet", "comfy", "comic", "comma", "conch", "condo", "coney", "conic", "conte", "copse", "coral", "corer", "corny", "cosmo", "costa", "couch", "cough", "could", "count", "coupe", "court", "coven", "cover", "covet", "covey", "covid", "cowan", "cower", "coyly", "crack", "craft", "craig", "cramp", "crane", "crank", "crash", "crass", "crate", "crave", "crawl", "craze", "crazy", "creak", "cream", "credo", "creed", "creek", "creep", "creme", "crepe", "crept", "cress", "crest", "crick", "cried", "crier", "crime", "crimp", "crisp", "croak", "crock", "croft", "crone", "crony", "crook", "crore", "cross", "croup", "crowd", "crown", "crude", "cruel", "crumb", "crump", "crush", "crust", "crwth", "crypt", "cubic", "cumin", "cured", "curio", "curly", "curry", "curse", "curve", "curvy", "cutie", "cyber", "cycle", "cynic"
};

const char *D_WORDS_5[] = {
    "daddy", "daily", "dairy", "daisy", "dally", "dance", "dandy", "darby", "darcy", "dared", "dated", "datum", "daunt", "dazed", "dealt", "death", "debar", "debit", "debug", "debut", "decal", "decay", "decor", "decoy", "decry", "defer", "deign", "deity", "delay", "delta", "delve", "demon", "demur", "denim", "dense", "depot", "depth", "derby", "deter", "detox", "deuce", "devil", "devon", "diary", "diced", "dicey", "digit", "dildo", "dilly", "dimly", "diner", "dingo", "dingy", "diode", "dirge", "dirty", "disco", "ditch", "ditto", "ditty", "diver", "dizzy", "dodge", "dodgy", "doggy", "dogma", "doing", "dolce", "dolly", "donna", "donor", "donut", "dopey", "doubt", "dough", "dowdy", "dowel", "downy", "dowry", "dozen", "draft", "drain", "drake", "drama", "drank", "drape", "drawl", "drawn", "dread", "dream", "dress", "dried", "drier", "drift", "drill", "drink", "drive", "droid", "droit", "droll", "drone", "drool", "droop", "drove", "drown", "druid", "drunk", "dryer", "dryly", "duchy", "dully", "dummy", "dumpy", "dunce", "dunno", "duped", "dusky", "dusty", "dutch", "duvet", "dwarf", "dwell", "dwelt", "dying"
};

 const char *E_WORDS_5[] = {
    "eager", "eagle", "early", "earth", "eased", "easel", "eaten", "eater", "ebony", "eclat", "edged", "edict", "edify", "eerie", "egret", "eight", "eject", "eking", "elate", "elbow", "elder", "elect", "elegy", "elfin", "elide", "elite", "elope", "elude", "email", "embed", "ember", "emcee", "emery", "emoji", "empty", "enact", "ended", "endow", "enema", "enemy", "enjoy", "ennui", "ensue", "enter", "entry", "envoy", "epoch", "epoxy", "equal", "equip", "erase", "erect", "erica", "erode", "error", "erupt", "essay", "ester", "ethel", "ether", "ethic", "ethos", "etude", "evade", "event", "every", "evict", "evoke", "exact", "exalt", "excel", "exert", "exile", "exist", "expat", "expel", "extol", "extra", "exult", "eying"
};

 const char *F_WORDS_5[] = {
    "fable", "faced", "facet", "facto", "faded", "faint", "fairy", "faith", "faked", "false", "famed", "fancy", "fanny", "farce", "fared", "fatal", "fated", "fatty", "fault", "fauna", "favor", "feast", "fecal", "feces", "feign", "fella", "felon", "femme", "femur", "fence", "feral", "ferry", "fetal", "fetch", "fetid", "fetus", "fever", "fewer", "fiber", "fibre", "field", "fiend", "fiery", "fifth", "fifty", "fight", "filed", "filer", "filet", "filly", "filmy", "filth", "final", "finch", "fined", "finer", "fired", "first", "firth", "fishy", "fitch", "fixed", "fixer", "fizzy", "fjord", "flack", "flail", "flair", "flake", "flaky", "flame", "flank", "flare", "flash", "flask", "fleck", "fleet", "flesh", "flick", "flier", "fling", "flint", "flirt", "float", "flock", "flood", "floor", "flora", "floss", "flour", "flout", "flown", "fluff", "fluid", "fluke", "flume", "flung", "flunk", "flush", "flute", "flyer", "foamy", "focal", "focus", "foggy", "foist", "folio", "folly", "footy", "foray", "force", "forge", "forgo", "forte", "forth", "forty", "forum", "found", "foyer", "frail", "frame", "franc", "frank", "fraud", "freak", "freed", "freer", "fresh", "friar", "fried", "frill", "frisk", "fritz", "frock", "frond", "front", "frost", "froth", "frown", "froze", "fruit", "fudge", "fugue", "fully", "fungi", "funky", "funny", "furor", "furry", "fused", "fussy", "fuzzy"
};

 const char *G_WORDS_5[] = {
    "gabby", "gable", "gaffe", "gaily", "gamer", "gamma", "gamut", "garth", "gassy", "gated", "gator", "gaudy", "gauge", "gaunt", "gauze", "gavel", "gawky", "gayer", "gayly", "gazer", "gecko", "geeky", "geese", "gemma", "genie", "genoa", "genre", "genus", "ghost", "ghoul", "giant", "giddy", "gimme", "girly", "girth", "given", "giver", "glade", "gland", "glare", "glass", "glaze", "gleam", "glean", "glide", "glint", "gloat", "globe", "glock", "gloom", "glory", "gloss", "glove", "glued", "glyph", "gnash", "gnome", "godly", "going", "golem", "golly", "gonad", "goner", "gonna", "goody", "gooey", "goofy", "goose", "gorge", "gotta", "gouge", "gourd", "grace", "grade", "graft", "grail", "grain", "grand", "grant", "grape", "graph", "grasp", "grass", "grate", "grave", "gravy", "graze", "great", "greed", "green", "greet", "grief", "grill", "grime", "grimy", "grind", "gripe", "groan", "groin", "groom", "grope", "gross", "group", "grout", "grove", "growl", "grown", "gruel", "gruff", "grunt", "guard", "guava", "guess", "guest", "guide", "guild", "guile", "guilt", "guise", "gulch", "gully", "gumbo", "gummy", "guppy", "gusto"
};

 const char *H_WORDS_5[] = {
    "habit", "haiku", "hairy", "halal", "halve", "handy", "happy", "haram", "hardy", "harem", "harpy", "harry", "harsh", "haste", "hasty", "hatch", "hated", "hater", "haunt", "haute", "haven", "havoc", "hazel", "heady", "heard", "heart", "heath", "heave", "heavy", "hedge", "hefty", "heist", "helix", "hella", "hello", "hence", "henry", "heron", "hertz", "hijab", "hilly", "hinge", "hippo", "hippy", "hired", "hitch", "hoard", "hobby", "hodge", "hogan", "hoist", "holed", "holly", "homer", "homie", "honda", "honed", "honey", "honor", "hoped", "horde", "horny", "horse", "hotel", "hotly", "hound", "house", "hovel", "hover", "howdy", "hubby", "human", "humid", "humor", "humph", "humus", "hunch", "hunky", "hurry", "husky", "hussy", "hutch", "hydra", "hydro", "hyena", "hymen", "hyped", "hyper"
};

 const char *I_WORDS_5[] = {
    "icily", "icing", "ideal", "idiom", "idiot", "idler", "idyll", "igloo", "iliac", "image", "imbue", "impel", "imply", "inane", "inbox", "incur", "index", "indie", "inept", "inert", "infer", "ingot", "inked", "inlay", "inlet", "inner", "input", "intel", "inter", "intro", "ionic", "irate", "irene", "irony", "islet", "issue", "itchy", "ivory"
};

 const char *J_WORDS_5[] = {
    "japan", "jaunt", "jazzy", "jelly", "jenny", "jerky", "jetty", "jewel", "jiffy", "jihad", "jimmy", "joint", "joist", "joked", "joker", "jolly", "josef", "josie", "joust", "judge", "juice", "juicy", "julio", "jumbo", "jumpy", "junta", "junto", "juror"
};

 const char *K_WORDS_5[] = {
    "kanji", "kappa", "kaput", "karma", "kayak", "kebab", "kelly", "kerry", "khaki", "kiddo", "kinda", "kinky", "kiosk", "kirby", "kitty", "knack", "knave", "knead", "kneed", "kneel", "knelt", "knife", "knock", "knoll", "known", "koala", "kodak", "kraft", "krill", "kylie", "kyrie"
};

 const char *L_WORDS_5[] = {
    "label", "labor", "laced", "lacey", "laden", "ladle", "lager", "laird", "lance", "lanky", "lapel", "lapse", "large", "larry", "larva", "laser", "lasso", "latch", "later", "latex", "lathe", "latte", "laugh", "laura", "layer", "leach", "leafy", "leaky", "leant", "leapt", "learn", "lease", "leash", "least", "leave", "ledge", "leech", "leery", "lefty", "legal", "leggy", "legit", "lemon", "lemur", "leone", "leper", "level", "lever", "levin", "liang", "libel", "liege", "light", "liked", "liken", "lilac", "limbo", "limit", "lined", "linen", "liner", "lingo", "lipid", "liter", "lithe", "litre", "lived", "liver", "livid", "llama", "loamy", "loath", "lobby", "local", "locus", "lodge", "lofty", "logic", "lohan", "loner", "loopy", "loose", "lorry", "loser", "lotta", "lotto", "lotus", "louie", "louse", "lousy", "loved", "lover", "lower", "lowly", "loyal", "lucid", "lucky", "lumen", "lumpy", "lunar", "lunch", "lunge", "lupus", "lurch", "lured", "lurid", "lusty", "lying", "lymph", "lynch", "lyric"
};

 const char *M_WORDS_5[] = {
    "macaw", "macho", "macon", "macro", "madam", "madly", "magic", "magma", "magna", "mahal", "maize", "major", "maker", "malik", "mambo", "mamma", "mammy", "manga", "mange", "mango", "mangy", "mania", "manic", "manly", "manor", "maple", "march", "marge", "maria", "marry", "marsh", "mason", "masse", "match", "mater", "matey", "matte", "mauve", "maxim", "maybe", "mayor", "mealy", "meant", "meaty", "mecca", "medal", "media", "medic", "melee", "melon", "merch", "mercy", "merge", "merit", "merle", "merry", "messy", "metal", "meter", "metre", "metro", "micro", "midge", "midst", "might", "milky", "mimic", "mince", "mined", "miner", "minim", "minor", "minty", "minus", "mirth", "miser", "missy", "misty", "mitch", "mixed", "mixer", "mocha", "modal", "model", "modem", "mogul", "moira", "moist", "molar", "moldy", "molly", "momma", "mommy", "monde", "money", "monte", "month", "moody", "moose", "moral", "moray", "moron", "morph", "mossy", "motel", "motif", "motor", "motto", "mould", "moult", "mound", "mount", "mourn", "mouse", "mouth", "moved", "mover", "movie", "mower", "mucky", "mucus", "muddy", "mulch", "mummy", "munch", "mural", "murky", "mushy", "music", "musky", "musty", "muted", "myrrh"
};

 const char *N_WORDS_5[] = {
    "nacho", "nadir", "naive", "naked", "named", "nancy", "nanny", "nasal", "nasty", "natal", "naval", "navel", "needy", "neigh", "nelly", "nerdy", "nerve", "neuro", "never", "newer", "newly", "nexus", "nicer", "niche", "nicky", "niece", "nifty", "nigga", "night", "ninja", "ninny", "ninth", "nitro", "noble", "nobly", "noise", "noisy", "nomad", "noose", "norma", "north", "nosed", "nosey", "notch", "noted", "novel", "nudge", "nurse", "nutty", "nylon", "nymph"
};

 const char *O_WORDS_5[] = {
    "oaken", "oasis", "obese", "occur", "ocean", "octal", "octet", "odder", "oddly", "offal", "offer", "often", "oiled", "olden", "older", "olive", "ollie", "ombre", "omega", "onion", "onset", "opera", "opine", "opium", "opted", "optic", "orbit", "order", "organ", "other", "otter", "ought", "ounce", "outdo", "outer", "outgo", "outta", "ovary", "ovate", "overt", "ovine", "ovoid", "owing", "owned", "owner", "oxide", "ozone"
};

 const char *P_WORDS_5[] = {
    "pablo", "paced", "paddy", "padre", "pagan", "paint", "paleo", "paler", "palsy", "panda", "panel", "panic", "pansy", "paolo", "papal", "paper", "parer", "parka", "parry", "parse", "party", "pasha", "pasta", "paste", "pasty", "patch", "patel", "patio", "patsy", "patty", "pause", "paved", "payee", "payer", "peace", "peach", "pearl", "pecan", "pedal", "peggy", "penal", "pence", "penis", "penne", "penny", "perch", "peril", "perky", "perry", "pesky", "pesto", "petal", "peter", "petit", "petty", "phase", "phone", "phony", "photo", "piano", "picky", "piece", "piety", "piggy", "piled", "pilot", "pinch", "piney", "pinky", "pinot", "pinto", "pious", "piper", "pique", "pitch", "pithy", "pivot", "pixel", "pixie", "pizza", "place", "plaid", "plain", "plait", "plane", "plank", "plant", "plate", "playa", "plaza", "plead", "pleat", "plied", "pluck", "plumb", "plume", "plump", "plunk", "plush", "poesy", "point", "poise", "poked", "poker", "polar", "polio", "polka", "polly", "polyp", "pooch", "poppy", "porch", "porno", "posed", "poser", "posit", "posse", "potty", "pouch", "pound", "pouty", "power", "prank", "prawn", "preen", "press", "price", "prick", "pride", "pried", "prima", "prime", "primo", "print", "prior", "prism", "privy", "prize", "probe", "promo", "prone", "prong", "proof", "prose", "proud", "prove", "prowl", "proxy", "prude", "prune", "psalm", "psych", "pubic", "pudgy", "puffy", "pulpy", "pulse", "punch", "pupal", "pupil", "puppy", "puree", "purer", "purge", "purse", "pushy", "pussy", "putty", "pygmy"
};

 const char *Q_WORDS_5[] = {
    "quack", "quail", "quake", "qualm", "quark", "quart", "quash", "quasi", "queen", "queer", "quell", "query", "quest", "queue", "quick", "quiet", "quill", "quilt", "quirk", "quite", "quota", "quote", "quoth"
};

 const char *R_WORDS_5[] = {
    "rabbi", "rabid", "raced", "racer", "radar", "radii", "radio", "raged", "rainy", "raise", "rajah", "rally", "ralph", "ramen", "ramon", "ranch", "randy", "range", "raped", "rapid", "rarer", "raspy", "rated", "ratio", "ratty", "raven", "rayon", "razor", "reach", "react", "ready", "realm", "rearm", "rebar", "rebel", "rebus", "rebut", "recap", "recon", "recur", "recut", "reedy", "refer", "refit", "regal", "rehab", "reign", "relax", "relay", "relic", "remit", "remix", "renal", "renew", "repay", "repel", "reply", "rerun", "reset", "resin", "retch", "retro", "retry", "reuse", "revel", "revue", "rhino", "rhyme", "rider", "ridge", "rifle", "right", "rigid", "rigor", "riley", "rinse", "ripen", "riper", "risen", "riser", "risky", "rival", "river", "rivet", "roach", "roast", "robin", "robot", "roche", "rocky", "rodeo", "roger", "rogue", "rohan", "roman", "rondo", "roomy", "roost", "roper", "rotor", "rouge", "rough", "round", "rouse", "route", "rover", "rowan", "rowdy", "rower", "royal", "rubin", "ruddy", "ruder", "rufus", "rugby", "ruled", "ruler", "rumba", "rumor", "rupee", "rural", "rusty", "ryder"
};

 const char *S_WORDS_5[] = {
    "saber", "sabha", "sabre", "sadly", "safer", "sahib", "saint", "salad", "salah", "salle", "sally", "salon", "salsa", "salty", "salve", "salvo", "samba", "sammy", "sandy", "saner", "santa", "santo", "sappy", "sassy", "satin", "satyr", "sauce", "saucy", "sauna", "saute", "saved", "saver", "savor", "savoy", "savvy", "scala", "scald", "scale", "scalp", "scaly", "scamp", "scant", "scare", "scarf", "scary", "scene", "scent", "scion", "scoff", "scold", "scone", "scoop", "scope", "score", "scorn", "scour", "scout", "scowl", "scram", "scrap", "scree", "screw", "scrub", "scrum", "scuba", "sedan", "seedy", "segue", "seine", "seize", "semen", "sense", "sepia", "serge", "serif", "serum", "serve", "setup", "seven", "sever", "sewer", "shack", "shade", "shady", "shaft", "shake", "shaky", "shale", "shall", "shalt", "shame", "shank", "shape", "shard", "share", "shark", "sharp", "shave", "shawl", "shear", "sheen", "sheep", "sheer", "sheet", "sheik", "shelf", "shell", "shied", "shift", "shine", "shiny", "shire", "shirk", "shirt", "shiva", "shoal", "shock", "shone", "shook", "shoot", "shore", "shorn", "short", "shout", "shove", "shown", "showy", "shred", "shrew", "shrub", "shrug", "shuck", "shunt", "shush", "shyly", "sided", "siege", "sieve", "sight", "sigma", "silky", "silly", "silva", "since", "sinew", "singe", "sinus", "siren", "sissy", "sixth", "sixty", "sized", "skate", "skier", "skiff", "skill", "skimp", "skirt", "skulk", "skull", "skunk", "slack", "slade", "slain", "slang", "slant", "slash", "slate", "slave", "sleek", "sleep", "sleet", "slept", "slice", "slick", "slide", "slime", "slimy", "sling", "slink", "sloop", "slope", "slosh", "sloth", "slump", "slung", "slunk", "slurp", "slush", "slyly", "smack", "small", "smart", "smash", "smear", "smell", "smelt", "smile", "smirk", "smite", "smith", "smock", "smoke", "smoky", "smote", "snack", "snail", "snake", "snaky", "snare", "snarl", "sneak", "sneer", "snide", "sniff", "snipe", "snoop", "snore", "snort", "snout", "snowy", "snuck", "snuff", "soapy", "sober", "soggy", "solar", "solid", "solve", "sonar", "sonic", "sonny", "sooth", "sooty", "sorry", "sorta", "sound", "south", "sower", "space", "spade", "spank", "spare", "spark", "spasm", "spawn", "speak", "spear", "speck", "speed", "spell", "spelt", "spend", "spent", "sperm", "spice", "spicy", "spied", "spiel", "spike", "spiky", "spill", "spilt", "spine", "spiny", "spire", "spite", "splat", "split", "spoil", "spoke", "spoof", "spook", "spool", "spoon", "spore", "sport", "spout", "spray", "spree", "sprig", "spunk", "spurn", "spurt", "squad", "squat", "squib", "squid", "stack", "staff", "stage", "staid", "stain", "stair", "stake", "stale", "stalk", "stall", "stamp", "stand", "stank", "stare", "stark", "start", "stash", "state", "stave", "stead", "steak", "steal", "steam", "steed", "steel", "steep", "steer", "stein", "stern", "stick", "stiff", "still", "stilt", "sting", "stink", "stint", "stock", "stoic", "stoke", "stole", "stomp", "stone", "stony", "stood", "stool", "stoop", "store", "stork", "storm", "story", "stout", "stove", "strap", "straw", "stray", "strip", "strut", "stuck", "study", "stuff", "stump", "stung", "stunk", "stunt", "style", "suave", "suede", "sugar", "suing", "suite", "sulky", "sully", "sumac", "sunny", "super", "supra", "surer", "surge", "surly", "susan", "sushi", "swami", "swamp", "swarm", "swash", "swath", "swear", "sweat", "swede", "sweep", "sweet", "swell", "swept", "swift", "swill", "swine", "swing", "swipe", "swirl", "swish", "swoon", "swoop"
};

 const char *T_WORDS_5[] = {
    "tabby", "table", "taboo", "tacit", "tacky", "taffy", "taint", "taken", "taker", "takin", "tally", "talon", "tamed", "tamer", "tammy", "tango", "tangy", "tanya", "taped", "taper", "tapir", "tardy", "tarot", "taste", "tasty", "tatty", "taunt", "tawny", "taxed", "teach", "teary", "tease", "teddy", "teeny", "teeth", "telly", "tempo", "tempt", "tenet", "tenor", "tense", "tenth", "tepee", "tepid", "terra", "terry", "terse", "tesla", "testy", "thank", "theft", "their", "theme", "there", "these", "theta", "thick", "thief", "thigh", "thine", "thing", "think", "third", "thong", "thorn", "those", "three", "threw", "throb", "throw", "thrum", "thumb", "thump", "thyme", "tiara", "tibet", "tibia", "tidal", "tiger", "tight", "tilde", "timed", "timer", "timid", "tipsy", "tired", "titan", "tithe", "title", "titty", "toast", "today", "toddy", "token", "tonal", "toned", "toner", "tonga", "tonic", "tonne", "tooth", "topaz", "topic", "torch", "torso", "torus", "total", "totem", "touch", "tough", "towed", "towel", "tower", "toxic", "toxin", "trace", "track", "tract", "trade", "trail", "train", "trait", "tramp", "trash", "trawl", "tread", "treat", "trend", "triad", "trial", "tribe", "trice", "trick", "tried", "tripe", "trite", "troll", "troop", "trope", "trout", "trove", "truce", "truck", "truer", "truly", "trump", "trunk", "truss", "trust", "truth", "tryst", "tubal", "tuber", "tulip", "tulle", "tummy", "tumor", "tuned", "tunic", "turbo", "tutor", "twain", "twang", "tweak", "tweed", "tweet", "twice", "twine", "twirl", "twist", "twixt", "tying", "typed"
};

 const char *U_WORDS_5[] = {
    "udder", "ulcer", "ultra", "umbra", "uncle", "uncut", "under", "undid", "undue", "unfed", "unfit", "unify", "union", "unite", "unity", "unlit", "unmet", "unset", "untie", "until", "unwed", "unzip", "upped", "upper", "upset", "urban", "urged", "urine", "usage", "usher", "using", "usual", "usurp", "utile", "utter"
};

 const char *V_WORDS_5[] = {
    "vague", "valet", "valid", "valor", "value", "valve", "vapid", "vapor", "vault", "vaunt", "vegan", "venom", "venue", "verge", "verse", "verso", "verve", "vicar", "video", "vigil", "vigor", "villa", "vinyl", "viola", "viper", "viral", "virus", "visit", "visor", "vista", "vital", "vitro", "vivid", "vixen", "vocal", "vodka", "vogue", "voice", "voila", "vomit", "voted", "voter", "vouch", "vowed", "vowel", "vying"
};

 const char *W_WORDS_5[] = {
    "wacky", "wafer", "waged", "wager", "wagon", "waist", "waive", "wally", "waltz", "wanna", "warty", "waste", "watch", "water", "waved", "waver", "waxed", "waxen", "weary", "weave", "weber", "wedge", "weedy", "weigh", "weird", "welch", "welsh", "wench", "whack", "whale", "wharf", "wheat", "wheel", "whelp", "where", "which", "whiff", "while", "whine", "whiny", "whirl", "whisk", "white", "whole", "whoop", "whore", "whose", "widen", "wider", "widow", "width", "wield", "wigan", "wight", "willy", "wimpy", "wince", "winch", "windy", "wiped", "wired", "wiser", "wispy", "witch", "witty", "woken", "woman", "women", "woody", "wooer", "wooly", "woozy", "wordy", "world", "worry", "worse", "worst", "worth", "would", "wound", "woven", "wrack", "wrath", "wreak", "wreck", "wrest", "wring", "wrist", "write", "wrong", "wrote", "wrung", "wryly"
};

 const char *X_WORDS_5[] = {
    "xerox"
};

 const char *Y_WORDS_5[] = {
    "yacht", "yahoo", "yearn", "yeast", "yield", "yikes", "young", "youth", "yummy"
};

 const char *Z_WORDS_5[] = {
    "zebra", "zesty", "zonal", "zoned", "zylyl"
};

const int SIZES[] = {
    sizeof(A_WORDS_5) / sizeof(A_WORDS_5[0]), sizeof(B_WORDS_5) / sizeof(B_WORDS_5[0]), sizeof(C_WORDS_5) / sizeof(C_WORDS_5[0]), sizeof(D_WORDS_5) / sizeof(D_WORDS_5[0]), sizeof(E_WORDS_5) / sizeof(E_WORDS_5[0]), sizeof(F_WORDS_5) / sizeof(F_WORDS_5[0]), sizeof(G_WORDS_5) / sizeof(G_WORDS_5[0]), sizeof(H_WORDS_5) / sizeof(H_WORDS_5[0]), sizeof(I_WORDS_5) / sizeof(I_WORDS_5[0]), sizeof(J_WORDS_5) / sizeof(J_WORDS_5[0]), sizeof(K_WORDS_5) / sizeof(K_WORDS_5[0]), sizeof(L_WORDS_5) / sizeof(L_WORDS_5[0]), sizeof(M_WORDS_5) / sizeof(M_WORDS_5[0]), sizeof(N_WORDS_5) / sizeof(N_WORDS_5[0]), sizeof(O_WORDS_5) / sizeof(O_WORDS_5[0]), sizeof(P_WORDS_5) / sizeof(P_WORDS_5[0]), sizeof(Q_WORDS_5) / sizeof(Q_WORDS_5[0]), sizeof(R_WORDS_5) / sizeof(R_WORDS_5[0]), sizeof(S_WORDS_5) / sizeof(S_WORDS_5[0]), sizeof(T_WORDS_5) / sizeof(T_WORDS_5[0]), sizeof(U_WORDS_5) / sizeof(U_WORDS_5[0]), sizeof(V_WORDS_5) / sizeof(V_WORDS_5[0]), sizeof(W_WORDS_5) / sizeof(W_WORDS_5[0]), sizeof(X_WORDS_5) / sizeof(X_WORDS_5[0]), sizeof(Y_WORDS_5) / sizeof(Y_WORDS_5[0]), sizeof(Z_WORDS_5) / sizeof(Z_WORDS_5[0])
};

const char **ALL_WORDS[] = {
    A_WORDS_5, B_WORDS_5, C_WORDS_5, D_WORDS_5, E_WORDS_5, F_WORDS_5, G_WORDS_5, H_WORDS_5, I_WORDS_5, J_WORDS_5, K_WORDS_5, L_WORDS_5, M_WORDS_5, N_WORDS_5, O_WORDS_5, P_WORDS_5, Q_WORDS_5, R_WORDS_5, S_WORDS_5, T_WORDS_5, U_WORDS_5, V_WORDS_5, W_WORDS_5, X_WORDS_5, Y_WORDS_5, Z_WORDS_5
};