fn main() {
    let base_sequence = "GAGTACAATTACATGATTACAAT".to_string();
    let sequence_fragment = "GATTACA".to_string();
    align(base_sequence, sequence_fragment);
    chop_fragment()
}

fn align(base_sequence: String, sequence_fragment: String) {
    let mut sequence = base_sequence;
    let mut extension = "-".repeat(sequence_fragment.len() - 1);
    let search_fragment = &sequence_fragment;

    sequence.push_str(&extension);
    extension.push_str(&sequence);

    let winner = (0..extension.len() - (sequence_fragment.len() - 1))
        .into_iter()
        .map(|i| {
            let referance_fragment = &extension[i..i + sequence_fragment.len()];
            println!("{}", &extension[i..i + sequence_fragment.len()]);
            let score = referance_fragment
                .as_bytes()
                .iter()
                .zip(search_fragment.as_bytes().iter())
                .filter(|(x, y)| x == y)
                .count();
            (i, score)
        })
        .max_by(|(_, a), (_, b)| a.cmp(b));

    let mut alignment_ = " ".repeat(winner.unwrap().0);
    alignment_.push_str(&sequence_fragment);
    println!("{}", extension);
    println!("{}", alignment_);
}

fn chop_fragment() {
    1
}